import ntp.commands
import importlib


class Command(ntp.commands.Command):
    
    def __init__(self):
        self.name = 'help'

    def run(self, args):
        print(args)
        return 0
