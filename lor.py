# coding: utf-8
#!/usr/bin/python
# Para rodar esse script é necessário pygal:
#   $ pip install pygal
import pygal
import sys
import math

def draw_graph(histogram, filename, num_lines, delta_lines, num_pages, delta_pages):
    line_chart = pygal.StackedLine(fill=True)
    line_chart.title = "Page index X Access - locality of reference"
    line_chart.x_labels = ["10^{}".format(p) for p in range(len(histogram))]
    total = 0.1 * num_lines
    for p in range(len(histogram[0])):
        line_chart.add(">{}".format((p+1)*delta_pages), [(histogram[l][p] * 100.0 / total) for l in range(len(histogram))])
    line_chart.render_to_file('{}_lor.svg'.format(filename[:-4]))

if __name__ == '__main__':
    page_size = 4

    perc_pages = 0.1
    num_pages = 2 ** (32 - int(math.log(page_size*1024, 2)))
    delta_pages = int(perc_pages * num_pages)

    perc_lines = 0.1
    num_lines = sum(1 for line in open(sys.argv[1]))
    delta_lines = int(perc_lines * num_lines)

    histogram = [[0 for i in range(int(1 / perc_pages))] for i in range(int(1 / perc_lines))]

    print "Number of memory accesses:", num_lines
    print "Number of virtual pages:", num_pages
    print "Delta:", delta_pages, "({} %)".format(perc_pages * 100)

    with open(sys.argv[1]) as f:
        for i, line in enumerate(f):
            page_index = int(line.split(' ')[0], base=16) >> (int(math.log(page_size*1024, 2)))
            histogram[int(i / delta_lines)][int(page_index / delta_pages)] += 1

    draw_graph(histogram, sys.argv[1], num_lines, delta_lines, num_pages, delta_pages)
