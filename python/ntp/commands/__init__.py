
class Command:
    """Base class for all NTP commands"""
    REQUIRE_GRID_CERT = False
    
    def __init__(self):
        pass
    
    def _execute(self):
        pass
    
    def run(self, params, args):
        pass
    
    def help(self):
        """Returns a little help text with the description of the command."""
        return ' ' + self.name + (' ' * (15 - len(self.name))) + self.shortDescription 
    
    def parse_arguments(self, name, argv):
        pass
    
    def get_text(self):
        return "IMPLEMENT ME"
        