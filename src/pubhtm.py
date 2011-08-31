from docutils.core import publish_file


output = publish_file(source_path="./tmp.tht", destination_path="./tmp.html", writer_name='html')
#output = publish_string(a, writer_name="html")
#print output
