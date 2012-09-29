from docutils.core import publish_file


output = publish_file(source_path="./tmp.tht", destination_path="./tmp.html", writer_name='html')
