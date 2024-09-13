#!/usr/bin/env python3

import argparse
import io
import logging
import os
import re
import shutil
import signal
import sys
import time
import unittest
from glob import glob
from multiprocessing import Process
from os.path import abspath, relpath
from subprocess import PIPE, STDOUT, Popen
from unittest import TestLoader, TextTestResult, TextTestRunner
from unittest.mock import patch

import pandas as pd

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
)


class BenchRunner:
    def __init__(self, base_dir="build/Release"):
        """
        Initialize the BenchRunner with a base directory for the benchmarks.
        """
        self.base_dir = base_dir
        self.benchmarks = {
            "coro_fibs": ["coro-fibs/coro-fibs"],
            "excret": ["excret/exc_ret", "excret/exc_ret_noexc"],
            "inline": ["inline/extswap"],
            "noexcept_qsort": [
                "noexcept-qsort/exc_partition",
                "noexcept-qsort/exc_qsort",
            ],
            "ranges_filter": ["ranges-filter/filter"],
            "ranges_projector": ["ranges-projector/projector"],
            "virtual_inherit": ["virtual-inherit/virtinh"],
            "virtual_inline": ["virtual-inline/virtinl"],
            "virtual_overhead": [
                "virtual-overhead/virtual",
                "virtual-overhead/virtual-shuffle",
            ],
        }

        if not os.path.exists(self.base_dir):
            logging.warning(
                f"Directory {self.base_dir} does not exist. Please ensure the build process has completed."
            )
            sys.exit(1)

        signal.signal(signal.SIGINT, self._handle_signal)

    def _handle_signal(self, sig, frame):
        """
        Handles the SIGINT (Ctrl+C) signal and ensures the script stops gracefully.
        Deletes any empty CSV files.
        """
        logging.info(
            "Execution interrupted by user. Gracefully stopping the benchmarking process..."
        )
        csv_files = glob("./*.csv")
        for csv_file in csv_files:
            if os.path.getsize(csv_file) == 0:
                logging.info(f"Removing empty CSV file: {csv_file}")
                os.remove(csv_file)
        sys.exit(0)

    def clean_ansi_escape_sequences(self, text):
        """
        Removes ANSI escape sequences from the given text.
        """
        ansi_escape = re.compile(r"\x1B[@-_][0-?]*[ -/]*[@-~]")
        return ansi_escape.sub("", text)

    def list_benchmarks(self):
        """
        Returns a list of available benchmarks that can be run.
        """
        return list(self.benchmarks.keys())

    def run_benchmark(self, benchmark_name, quiet=False, histograms=None):
        """
        Runs a specified benchmark and captures relevant data in a CSV file.
        """
        if benchmark_name not in self.benchmarks:
            logging.warning(
                f"Benchmark '{benchmark_name}' is not in the list of available benchmarks."
            )
            return

        exec_paths = self.benchmarks.get(benchmark_name, [])
        for exec_path in exec_paths:
            full_path = os.path.join(self.base_dir, exec_path)
            if not os.path.isfile(full_path):
                logging.warning(
                    f"Executable for {benchmark_name} not found at {full_path}"
                )
                continue

            try:
                logging.info(f"Starting benchmark: {benchmark_name} at {full_path}")
                cmd = [full_path, "-o", "csv"]
                if quiet:
                    cmd.append("-q")
                if histograms:
                    cmd.extend(["-r", histograms])

                with Popen(
                    cmd, stdout=PIPE, stderr=STDOUT, bufsize=1, universal_newlines=True
                ) as process:
                    with open(f"./{benchmark_name}.csv", "w") as f:
                        header_written = False
                        for line in process.stdout:
                            if re.search(r"^\[\s*\d+%\]", line) or "Launching" in line:
                                print(f"\033[93m{line.strip()}\033[0m")
                            elif (
                                "name,avg_time,min_time" in line and not header_written
                            ):
                                f.write(line)
                                header_written = True
                            elif header_written and (
                                "total_operations" in line or "avg_time" in line
                            ):
                                continue
                            else:
                                clean_output = self.clean_ansi_escape_sequences(line)
                                f.write(clean_output)

                logging.info(f"Benchmark {benchmark_name} completed successfully.")
            except Exception as e:
                logging.error(f"Error running benchmark {benchmark_name}: {e}")

    def run_benchmarks(
        self,
        selected_benchmarks=None,
        filter_pattern=None,
        quiet=False,
        histograms=None,
    ):
        """
        Runs selected benchmarks based on the provided filters.
        If no benchmarks are provided, runs all available ones.
        """
        if not selected_benchmarks:
            selected_benchmarks = self.list_benchmarks()

        for benchmark in selected_benchmarks:
            for exec_path in self.benchmarks.get(benchmark, []):
                if filter_pattern and not re.search(filter_pattern, benchmark):
                    logging.info(f"Skipping benchmark {benchmark} due to filter")
                    continue
                full_path = os.path.join(self.base_dir, exec_path)
                self.run_benchmark(benchmark, quiet=quiet, histograms=histograms)

    def copy_csvs_to_output_dir(self, output_dir):
        """
        Copies all generated CSVs to the specified output directory.
        """
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
            logging.info(f"Created directory {output_dir}")

        csv_files = glob("./*.csv")
        for csv_file in csv_files:
            shutil.copy(csv_file, output_dir)
            logging.info(f"Copied {csv_file} to {output_dir}")


class BenchRunnerArgParser:
    """
    This class handles parsing the command-line arguments for the BenchRunner.
    """

    def __init__(self, runner):
        self.runner = runner
        self.parser = argparse.ArgumentParser(description="Run selected benchmarks.")
        self._add_arguments()

    def _add_arguments(self):
        """
        Adds all required arguments for the BenchRunner CLI interface.
        """
        self.parser.add_argument(
            "-f", "--filter", help="Filter benchmarks by the given regexp pattern"
        )
        self.parser.add_argument(
            "-l", "--list", action="store_true", help="List all available benchmarks"
        )
        self.parser.add_argument(
            "-q", "--quiet", action="store_true", help="Launch in quiet mode"
        )
        self.parser.add_argument(
            "-o",
            "--output-dir",
            help="Specify directory to copy generated CSVs after benchmarks",
        )
        self.parser.add_argument(
            "-r",
            "--histograms",
            help="Create High Dynamic Range (HDR) Histogram files with a given resolution",
        )
        self.parser.add_argument(
            "--version",
            action="store_true",
            help="Show program's version number and exit",
        )
        self.parser.add_argument(
            "--test", action="store_true", help="Run unit tests for BenchRunner"
        )

        for benchmark in self.runner.benchmarks:
            self.parser.add_argument(
                f"--{benchmark}",
                action="store_true",
                help=f"Run benchmarks from {benchmark}/",
            )

    def parse_args(self):
        """
        Parses the command-line arguments and returns the result.
        """
        return self.parser.parse_args()

    def handle_args(self, args):
        """
        Handles the parsed arguments and invokes the corresponding BenchRunner methods.
        """
        if args.version:
            print("Benchmark Runner v1.0")
            sys.exit(0)

        if args.test:
            print("Running unit tests for BenchRunner...")
            suite = unittest.TestLoader().loadTestsFromTestCase(BenchRunnerTesting)
            runner = CustomTextTestRunner(verbosity=2)
            runner.run(suite)
            return

        if args.list:
            print(self.runner.list_benchmarks())
            sys.exit(0)

        selected_benchmarks = [
            benchmark
            for benchmark in self.runner.benchmarks
            if getattr(args, benchmark)
        ]

        self.runner.run_benchmarks(
            selected_benchmarks=selected_benchmarks,
            filter_pattern=args.filter,
            quiet=args.quiet,
            histograms=args.histograms,
        )

        if args.output_dir:
            self.runner.copy_csvs_to_output_dir(args.output_dir)


class CustomTextTestRunner(TextTestRunner):
    def _makeResult(self):
        return CustomTestResult(self.stream, self.descriptions, self.verbosity)


class CustomTestResult(TextTestResult):
    def startTest(self, test):
        test_file = relpath(
            test.__module__.replace(".", "/") + ".py", start=abspath(".")
        )
        self.stream.write(f"{test_file}: ")
        super().startTest(test)

    def addSuccess(self, test):
        self.stream.write("\033[92m")  # Green for success
        super().addSuccess(test)
        self.stream.write("\033[0m")

    def addFailure(self, test, err):
        self.stream.write("\033[91m")  # Red for failure
        super().addFailure(test, err)
        self.stream.write("\033[0m")

    def addError(self, test, err):
        self.stream.write("\033[91m")  # Red for error
        super().addError(test, err)
        self.stream.write("\033[0m")

    def addSkip(self, test, reason):
        self.stream.write("\033[93m")  # Yellow for skipped tests
        super().addSkip(test, reason)
        self.stream.write("\033[0m")

    def addExpectedFailure(self, test, err):
        self.stream.write("\033[93m")  # Yellow for expected failures
        super().addExpectedFailure(test, err)
        self.stream.write("\033[0m")

    def addUnexpectedSuccess(self, test):
        self.stream.write("\033[92m")  # Green for unexpected success
        super().addUnexpectedSuccess(test)
        self.stream.write("\033[0m")


class BenchRunnerTesting(unittest.TestCase):

    @patch("os.path.exists", return_value=True)
    @patch("logging.warning")
    def test_base_dir_exists(self, mock_warning, mock_exists):
        runner = BenchRunner(base_dir="build/Release")
        self.assertFalse(mock_warning.called)

    @patch("os.path.exists", return_value=False)
    @patch("logging.warning")
    @patch("sys.exit")  # Preventing the program from exiting
    @patch("sys.stdout", new_callable=io.StringIO)  # Suppressing the output
    def test_base_dir_not_exists(
        self, mock_stdout, mock_exit, mock_warning, mock_exists
    ):
        runner = BenchRunner(base_dir="test_dir")
        mock_exit.assert_called_once_with(1)
        mock_warning.assert_called_once_with(
            "Directory test_dir does not exist. Please ensure the build process has completed."
        )

    @patch("os.path.isfile", return_value=False)
    @patch("logging.warning")
    def test_run_benchmark_found(self, mock_warning, mock_isfile):
        runner = BenchRunner(base_dir="build/Release")
        runner.run_benchmark("coro_fibs")

        mock_isfile.assert_called_once_with("build/Release/coro-fibs/coro-fibs")
        mock_warning.assert_called_once_with(
            "Executable for coro_fibs not found at build/Release/coro-fibs/coro-fibs"
        )

    @patch("logging.warning")
    def test_run_benchmark_invalid_name(self, mock_warning):
        invalid_name = "nonexistent_benchmark"

        runner = BenchRunner(base_dir="build/Release")
        runner.run_benchmark(invalid_name)

        mock_warning.assert_called_once_with(
            f"Benchmark '{invalid_name}' is not in the list of available benchmarks."
        )

    @patch("os.path.getsize")
    @patch("run_benchmarks.Popen")
    @patch("sys.stdout", new_callable=io.StringIO)
    @patch("logging.info")
    def test_sigint_removes_empty_csv(
        self, mock_info, mock_stdout, mock_popen, mock_getsize
    ):
        def run_benchmark_process():
            runner = BenchRunner(base_dir="build/Release")
            runner.run_benchmark("coro_fibs")

        def send_sigint_to_process(proc):
            time.sleep(1)
            os.kill(proc.pid, signal.SIGINT)

        mock_popen.return_value.__enter__.return_value.stdout = ["Benchmark running"]
        mock_getsize.side_effect = [0]

        # Start the benchmark process
        benchmark_process = Process(target=run_benchmark_process)
        benchmark_process.start()

        # Start another process to send SIGINT
        sigint_process = Process(
            target=send_sigint_to_process, args=(benchmark_process,)
        )
        sigint_process.start()

        # Wait for both processes to finish
        sigint_process.join()
        benchmark_process.join()

        self.assertFalse(os.path.exists("coro_fibs.csv"))

    @patch("os.path.isfile", return_value=True)
    @patch("run_benchmarks.Popen")
    @patch("logging.info")
    @patch("sys.stdout", new_callable=io.StringIO)
    def test_run_benchmark_valid_csv_content(
        self, mock_stdout, mock_info, mock_popen, mock_isfile
    ):
        mock_popen.return_value.__enter__.return_value.stdout = ["Benchmark running"]
        runner = BenchRunner(base_dir="build/Release")
        runner.run_benchmark("coro_fibs", quiet=True)

        csv_file = "coro_fibs.csv"
        self.assertTrue(os.path.exists(csv_file), "CSV file was not generated")
        df = pd.read_csv(csv_file)

        # Check the header row A1:K1 (column names)
        expected_columns = [
            "name",
            "avg_time",
            "min_time",
            "max_time",
            "total_time",
            "total_operations",
            "total_items",
            "total_bytes",
            "operations_per_second",
            "items_per_second",
            "bytes_per_second",
        ]
        self.assertEqual(
            list(df.columns),
            expected_columns,
            "CSV headers do not match expected values",
        )

        # Check the first column (A1:A3) contains correct string values
        expected_names = ["sumfib_coro", "sumfib"]
        self.assertEqual(df["name"].iloc[0], "sumfib_coro")
        self.assertEqual(df["name"].iloc[1], "sumfib")

        # Check that the fields B2:K3 contain numeric values
        numeric_columns = df.columns[1:]
        for index, row in df.iterrows():
            for col in numeric_columns:
                self.assertIsInstance(
                    row[col], (int, float), f"Value in {col} is not numeric"
                )

        if os.path.exists(csv_file):
            os.remove(csv_file)


if __name__ == "__main__":
    runner = BenchRunner()
    arg_parser = BenchRunnerArgParser(runner)
    args = arg_parser.parse_args()
    arg_parser.handle_args(args)
