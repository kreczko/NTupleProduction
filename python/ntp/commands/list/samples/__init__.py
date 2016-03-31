import ntp.commands

class Command(ntp.commands.list.Command):
    def run(self, params, args):
        print('Listing all known samples')
        
    def help(self):
        return "Lists all available samples"