import os
import sqlite3 as lite

dirs = [ '/mnt/samsung2/porn', '/mnt/samsung1/PORN', '/mnt/seagate/porn' ]
extens = ['avi', 'wmv', 'mkv', 'mp4']  # the extensions to search for

con = lite.connect('db')
con.text_factory = str

cur = con.cursor()
for topdir in dirs:
    for dirpath, dirnames, files in os.walk(topdir):
        for name in files:
            ext = name.lower().rsplit('.', 1)[-1]
        if ext in extens:
            #print(os.path.join(dirpath, name))
            print(os.path.splitext(name)[0])
            # vid = (name, os.path.join(dirpath, name))
            # cur.execute("INSERT INTO Vids(title,path) VALUES (?,?)", vid)

con.commit()
