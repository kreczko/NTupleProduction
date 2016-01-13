
class CompositeSelection():
    
    def __init__(self):
        self._selections = []
        
    def selects(self, event):
        grouped_selections = self.group_selections(self._selections)
        results = []
        for x, group in grouped_selections.items():
            if x == '':
                results.append(all([s._func(event) for s in group]))
            else:
                collection = eval('event.{0}'.format(x))
                collection_result = []
                for e in collection:
                    collection_result.append(all([s._func(e) for s in group]))
                r_type = max([s._type for s in group]) # instead of max, type should be resolved for all selections
                results.append(Selection.resolveType(collection_result, r_type))
        return all(results)
    
    #TODO: test
    def group_selections(self, selections):
        '''
            group selection by collection they use
        '''
        grouped_selections = {}
        # group selection by collection they use
        for s in selections:
            x = s._x
            if grouped_selections.has_key(x):
                grouped_selections[x].append(s)
            else:
                grouped_selections[x] = [s]
        return grouped_selections
    
    
    def add_selection(self, selection):
        # functions need to be added, not selections.
        # if selections work on same collections, they need to be coupled
        self._selections.append(selection)
        
    def __add__(self, other):
        if type(self) == type(other):
            self._selections.extend(other._selections)
        elif type(other) == 'Selection':
            self.add_selection(selection)
        else:
            print 'unknown type of selection'
        return self

class Selection():
    AtLeastOne = 1 # this can be lambda x: x>=1
    AtLeastTwo = 2 # this can be lambda x: x>=2
    def __init__(self, func, x = '', type = 1):
        self._func = func
        self._x = x
        self._type = type
        self.selections = [self]
        
    
    def selects(self, event):
        if self._x == '':
            return self._func(event)
        else:
            collection = eval('event.{0}'.format(self._x))
            result = []
            for x in collection:
                result.append(self._func(x))
            return Selection.resolveType(result, self._type)
        
    @staticmethod
    def resolveType(result, r_type):
        n_passed = result.count(True)
        return n_passed >= r_type
    
    def then(self, other_selection):
        s = CompositeSelection()
        s.add_selection(self)
        s.add_selection(other_selection)
        return s
    
    def __add__(self, other):
        return self.then(other)