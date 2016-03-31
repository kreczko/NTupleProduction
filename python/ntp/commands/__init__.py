
class Command:
    """Base class for all NTP commands"""
    
    def __init__(self):
        pass
    
    def _execute(self):
        pass
    
    def run(self, params, args):
        pass
    
    def help(self):
        """Returns a little help text with the description of the command."""
        return ' ' + self.name + (' ' * (15 - len(self.name))) + self.shortDescription 