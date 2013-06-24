# coding: utf-8
#!/usr/bin/python
# Para rodar esse script é necessário pygal:
#   $ pip install pygal
import pygal
import json

def load_json():
    with open("resultado.json", "r") as f:
        result = json.load(f)

    return result

def draw_graph(program, data):
    for rw in ["writes", "reads"]:
        custom_style = pygal.style.Style(
            colors=('#FF0000', '#00FF00', '#0000FF'))
        line_chart = pygal.Line(style=custom_style) #pygal.Line()
        line_chart.title = "Page size (KB) X Number of page {} ({})".format(rw, program)
        line_chart.x_labels = [str(h["tamanho"]) + "KB" for h in data[0]["resultado"]] #map(str, range(2002, 2013))
        for d in data:
            line_chart.add("{}".format(d["algoritmo"]), [int(h[rw]) for h in d["resultado"]])
        line_chart.render_to_file('{}_{}.svg'.format(program[:-4], rw))

if __name__ == '__main__':
    result = load_json()
    for program, data in result.items():
        draw_graph(program, data)
