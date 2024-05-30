#!/usr/bin/python3

import os
from collections import defaultdict
from collections import namedtuple
import csv

from matplotlib import pyplot as plt

TestInfo = namedtuple("Data", "cpu author compiler test subtest test_data")
TestData = namedtuple("TestData", "avg_time")

PATH_TO_RESULTS = './results'
OUTPUT_FILE = "results.jpg"

def parse_csv(path):
    with open(path, 'r', encoding='utf-8-sig', newline='') as file:
        reader = csv.reader(file)
        header = list(next(reader))
        subtests = [dict(zip(header,row)) for row in reader]
    return subtests

def get_data():
    file_list = defaultdict(list)
    for root, dirs, files in os.walk(PATH_TO_RESULTS):
        for file in files:
            path = os.path.join(root, file)
            test = file[:-4]
            splitted = path[len(PATH_TO_RESULTS):].split('/')
            if len(splitted) == 5: # work around
                start_index = -3
                compiler = splitted[-2]
            else:
                start_index = -2
                compiler = 'None'
            author = splitted[start_index]
            cpu = splitted[start_index - 1]

            subtests = parse_csv(path)
            for subtest in subtests:
                test_data = TestData(int(subtest['avg_time']))
                file_list[(test, subtest['name'])].append(TestInfo(cpu, author, compiler, test, subtest['name'], test_data))
    return file_list

def make_plot(test_data):
    test_count = len(test_data)
    measurements_count = len(test_data[next(iter(test_data))])

    fig, axes = plt.subplots(figsize=(20, measurements_count * test_count / 2), nrows=test_count)

    for i, (k, v) in enumerate(test_data.items()):
        names = [f"{data.cpu}/{data.author}/{data.compiler}" for data in v]
        values = [data.test_data.avg_time for data in v]
        ax = axes[i]

        ax.barh(names, values)

        for j in ax.patches:
            ax.text(j.get_width() + 0.2, j.get_y() + 0.5, 
                    str(round((j.get_width()), 2)),
                    fontsize=10, fontweight='bold',
                    color ='grey')
        
        ax.set_title(f"{k[0]}/{k[1]}", loc='left', )

    fig.savefig("results.jpg", bbox_inches='tight')

if __name__ == "__main__":
    test_data = get_data()
    make_plot(test_data)