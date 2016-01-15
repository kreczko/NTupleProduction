from __future__ import division
from collections import namedtuple, Counter

CriteriaTuple = namedtuple('Criteria', ['AtLeastOne', 'AtLeastTwo'])
Criteria = CriteriaTuple(lambda n: n >= 1, lambda n: n >= 2)


class Selection():

    def __init__(self, name='selection', func=None, x='', type=1):
        self._name = name
        self._func = func
        self._x = x
        self._type = type
        self._selections = [self]
        self._chain = []
        self._grouped_selections = {}
        self._grouped_selections[x] = [self]
        self._counter = Counter()

    def __add_selection__(self, selection):
        for s in selection._selections:
            x = s._x
            if self._grouped_selections.has_key(x):
                self._grouped_selections[x].append(s)
            else:
                self._grouped_selections[x] = [s]
        self._name += ' + ' + selection._name
        return self

    def selects(self, event):
        self._counter['processed'] += 1
        result = False
        if self._chain:
            result = self.__selects_with_chain__(event)
        else:
            result = self.__selects_combined__(event)
        if result:
            self._counter['passed'] += 1
        return result

    def __selects_with_chain__(self, event):
        return all([s.selects(event) for s in self._chain])

    def __selects_combined__(self, event):
        results = []
        grouped_selections = self._grouped_selections
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

    def then(self, other):
        s = Selection(name=self._name + ', ' + other._name)
        if self._chain:
            s._chain = self._chain
        else:
            s._chain = [self]
        s._chain.append(other)

        return s

    def __add__(self, other):
        return self.__add_selection__(other)

    def n_processed(self):
        return self._counter['processed']

    def n_passed(self):
        return self._counter['passed']

    def efficiency(self):
        return self.n_passed() / self.n_processed()

    def cutflow(self):
        selections = []
        if self._chain:
            selections = self._chain
        else:
            selections = [self]
        flow = []
        for i, s in enumerate(selections):
            if i == 0:
                flow.append(s.n_processed())
            flow.append(s.n_passed())
        return flow
