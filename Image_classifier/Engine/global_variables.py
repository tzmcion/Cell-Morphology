PATH = ""
ACCEPTED = [".JPG",".JPEG"]
OUTPUT = ""
MODE = "Copy"

class Binder:
    def __init__(self, key:str, name:str) -> None:
        self.key = key
        self.name = name

BINDERS = [Binder("1","Round"),Binder("2","Star"),Binder("3","Round"),Binder("-1","Not Resolvable")]