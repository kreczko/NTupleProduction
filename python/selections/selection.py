from collections import namedtuple

CriteriaTuple = namedtuple('Criteria', ['AtLeastOne', 'AtLeastTwo'])
Criteria = CriteriaTuple(lambda n: n >= 1, lambda n: n >= 2)

class Selection():

    def __init__(self, func, x='', type=1):
        self._func = func
        self._x = x
        self._type = type
        self._selections = [self]
        self._grouped_selections = {}
        self._grouped_selections[x] = [self]

    def __add_selection__(self, selection):
        for s in selection._selections:
            x = s._x
            if self._grouped_selections.has_key(x):
                self._grouped_selections[x].append(s)
            else:
                self._grouped_selections[x] = [s]
        return self

    def selects(self, event):
        grouped_selections = self._grouped_selections
        results = []
        for x, group in grouped_selections.items():
            if x == '':
                results.append(all([s._func(event) for s in group]))
            else:
                collection = eval('event.{0}'.format(x))
                collection_result = []
                for e in collection:
                    collection_result.append(all([s._func(e) for s in group]))
                r_type = Selection.resolve_type
                criteria = [r_type(collection_result, s._type) for s in group]
                type_result = all(criteria)
                results.append(type_result)
        return all(results)

    @staticmethod
    def resolve_type(result, r_type):
        n_passed = result.count(True)
        return r_type(n_passed)

    def then(self, other_selection):
        s = CompositeSelection()
        s.add_selection(self)
        s.add_selection(other_selection)
        return s

    def __add__(self, other):
        return self.__add_selection__(other)
