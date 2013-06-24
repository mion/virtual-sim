#!/usr/bin/python
# Para rodar esse script é necessário pygal:
#   $ pip install pygal
import pygal

line_chart = pygal.Line()
line_chart.title = 'compilador.log'
line_chart.x_labels = map(str, range(2002, 2013))
line_chart.add('NRU', [None, None, 0, 16.6,   25,   31, 36.4, 45.5, 46.3, 42.8, 37.1])
line_chart.add('NRU',  [None, None, None, None, None, None,    0,  3.9, 10.8, 23.8, 35.3])
line_chart.add('SEG',      [85.8, 84.6, 84.7, 74.5,   66, 58.6, 54.7, 44.8, 36.2, 26.6, 20.1])
