import os

def findfile(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)
        
findfile(HellowWorld1.py, Virus/scripts)
