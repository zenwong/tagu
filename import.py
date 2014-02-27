import sqlite3
import os

rootPath = '/mnt/seagate/japanese'
conn = sqlite3.connect('db')
c = conn.cursor()

for root, dirs, files in os.walk(rootPath):
  for filename in files:
    path = os.path.join(root, filename)
    name = os.path.splitext(filename)[0]
    v = (name,path)
    c.execute("INSERT INTO vids(title,path) VALUES (?,?)", v)

conn.commit()

c.execute('delete from search')
c.execute('insert into search(docid,title,tags,acts) select vid,title,tags,acts from LibraryView')
conn.commit()
