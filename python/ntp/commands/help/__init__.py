import ntp.commands
import importlib


class Command(ntp.commands.Command):

    def run(self, params, args):
        return True
