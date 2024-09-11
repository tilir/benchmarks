import os
from subprocess import Popen, PIPE, STDOUT
import argparse
import re
import signal
import sys
import logging
from glob import glob
import shutil

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
)

BASE_DIR = "build/Release"

benchmarks = {
    "coro_fibs": ["coro-fibs/coro-fibs"],
    "excret": ["excret/exc_ret", "excret/exc_ret_noexc"],
    "inline": ["inline/extswap"],
    "noexcept_qsort": ["noexcept-qsort/exc_partition", "noexcept-qsort/exc_qsort"],
    "ranges_filter": ["ranges-filter/filter"],
    "ranges_projector": ["ranges-projector/projector"],
    "virtual_inherit": ["virtual-inherit/virtinh"],
    "virtual_inline": ["virtual-inline/virtinl"],
    "virtual_overhead": [
        "virtual-overhead/virtual",
        "virtual-overhead/virtual-shuffle",
    ],
}


def clean_ansi_escape_sequences(text):
    """
    Removes ANSI escape sequences from the given text.

    Args:
        text (str): The text with potential ANSI escape sequences.

    Returns:
        str: Cleaned text without ANSI escape sequences.
    """
    ansi_escape = re.compile(r"\x1B[@-_][0-?]*[ -/]*[@-~]")
    return ansi_escape.sub("", text)


def help_msg():
    """
    Displays a help message for using the benchmark runner script.
    """
    msg = """
Usage: benchmark_runner.py [options]

Options:
  --version             Show program's version number and exit
  -h, --help            Show this help message and exit
  -f FILTER, --filter=FILTER
                        Filter benchmarks by the given regexp pattern
  -l, --list            List all available benchmarks
  -q, --quiet           Launch in quiet mode. No progress will be shown
  -r HISTOGRAMS, --histograms=HISTOGRAMS
                        Create High Dynamic Range (HDR) Histogram files with a
                        given resolution. Default: 0
  -o, --output-dir      Specify directory to copy generated CSVs after benchmarks
"""
    print(msg)


def list_benchmarks():
    """
    Lists all available benchmarks that can be run.
    """
    print("Available benchmarks:")
    for benchmark in benchmarks.keys():
        print(f"- {benchmark}")


def run_benchmark(exec_path, benchmark_name, quiet=False, histograms=None):
    """
    Runs the specified benchmark and captures relevant data in a CSV file.

    Args:
        exec_path (str): Path to the executable file for the benchmark.
        benchmark_name (str): The name of the benchmark being run.
        quiet (bool, optional): If True, run in quiet mode with no progress output. Defaults to False.
        histograms (str, optional): HDR Histogram resolution. Defaults to None.
    """
    try:
        if os.path.isfile(exec_path):
            logging.info(f"Starting benchmark: {benchmark_name} at {exec_path}")
            cmd = [exec_path, "-o", "csv"]
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
                        elif "name,avg_time,min_time" in line and not header_written:
                            f.write(line)
                            header_written = True
                        elif header_written and (
                            "total_operations" in line or "avg_time" in line
                        ):
                            continue
                        else:
                            clean_output = clean_ansi_escape_sequences(line)
                            f.write(clean_output)

            logging.info(f"Benchmark {benchmark_name} completed successfully.")
        else:
            logging.warning(
                f"Executable for {benchmark_name} not found at {exec_path}!"
            )
    except Exception as e:
        logging.error(f"Error running benchmark {benchmark_name}: {e}")


def run_benchmarks(
    selected_benchmarks, filter_pattern=None, quiet=False, histograms=None
):
    """
    Runs the selected benchmarks based on the provided flags and filters.

    Args:
        selected_benchmarks (list): List of benchmarks to be run.
        filter_pattern (str, optional): Regular expression pattern to filter benchmarks. Defaults to None.
        quiet (bool, optional): If True, run in quiet mode. Defaults to False.
        histograms (str, optional): HDR Histogram resolution. Defaults to None.
    """
    for benchmark in selected_benchmarks:
        for exec_path in benchmarks.get(benchmark, []):
            if filter_pattern and not re.search(filter_pattern, benchmark):
                logging.info(f"Skipping benchmark {benchmark} due to filter")
                continue
            full_path = os.path.join(BASE_DIR, exec_path)
            run_benchmark(full_path, benchmark, quiet, histograms)


def run_all_benchmarks(filter_pattern=None, quiet=False, histograms=None):
    """
    Recursively finds and runs all benchmarks from the build directory.

    Args:
        filter_pattern (str, optional): Regular expression pattern to filter benchmarks. Defaults to None.
        quiet (bool, optional): If True, run in quiet mode. Defaults to False.
        histograms (str, optional): HDR Histogram resolution. Defaults to None.
    """
    logging.info(f"Running all benchmarks from {BASE_DIR} recursively.")
    for root, dirs, files in os.walk(BASE_DIR):
        for file in files:
            exec_path = os.path.join(root, file)
            if os.access(exec_path, os.X_OK):
                benchmark_name = os.path.basename(root)
                if filter_pattern and not re.search(filter_pattern, benchmark_name):
                    logging.info(f"Skipping benchmark {benchmark_name} due to filter")
                    continue
                run_benchmark(exec_path, benchmark_name, quiet, histograms)


def copy_csvs_to_output_dir(output_dir):
    """
    Copies all generated CSVs to the specified output directory.
    Creates directories if they do not exist.

    Args:
        output_dir (str): The target directory to copy CSVs to.
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)
        path_parts = output_dir.split(os.sep)
        for i in range(1, len(path_parts) + 1):
            path_to_create = os.path.join(*path_parts[:i])
            if not os.path.exists(path_to_create):
                os.makedirs(path_to_create)
                logging.info(f"Created dir {path_to_create}")

    csv_files = glob("./*.csv")
    for csv_file in csv_files:
        shutil.copy(csv_file, output_dir)
        logging.info(f"Copied {csv_file} to {output_dir}")


def signal_handler(sig, frame):
    """
    Handles the SIGINT (Ctrl+C) signal and ensures the script stops gracefully. Deletes any empty CSV files.

    Args:
        sig (int): Signal number.
        frame (FrameType): Current stack frame.
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


if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)

    if not os.path.exists(BASE_DIR):
        logging.warning(
            f"Directory {BASE_DIR} does not exist. Please ensure the build process has completed."
        )
        sys.exit(1)

    parser = argparse.ArgumentParser(description="Run selected benchmarks.")
    parser.add_argument(
        "-f", "--filter", help="Filter benchmarks by the given regexp pattern"
    )
    parser.add_argument(
        "-l", "--list", action="store_true", help="List all available benchmarks"
    )
    parser.add_argument(
        "-q", "--quiet", action="store_true", help="Launch in quiet mode"
    )
    parser.add_argument(
        "-o",
        "--output-dir",
        help="Specify directory to copy generated CSVs after benchmarks",
    )
    parser.add_argument(
        "-r",
        "--histograms",
        help="Create High Dynamic Range (HDR) Histogram files with a given resolution",
    )
    parser.add_argument(
        "--version", action="store_true", help="Show program's version number and exit"
    )

    for benchmark in benchmarks:
        parser.add_argument(
            f"--{benchmark}",
            action="store_true",
            help=f"Run benchmarks from {benchmark}/",
        )

    args = parser.parse_args()

    if args.version:
        print("Benchmark Runner v1.0")
        sys.exit(0)

    if args.list:
        list_benchmarks()
        sys.exit(0)

    selected_benchmarks = [
        benchmark for benchmark in benchmarks if getattr(args, benchmark)
    ]

    if not selected_benchmarks:
        logging.info(
            "No specific benchmarks selected. Running all benchmarks recursively from build/Release/"
        )
        run_all_benchmarks(
            filter_pattern=args.filter, quiet=args.quiet, histograms=args.histograms
        )
    else:
        run_benchmarks(
            selected_benchmarks, filter_pattern=args.filter, quiet=args.quiet
        )

    if args.output_dir:
        copy_csvs_to_output_dir(args.output_dir)
