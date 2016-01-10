# Selections
Selections provide an easy way to specify how to select events. A selection contains one or more conditions:
```
# a selection that tests 'x.hadronicOverEm() < 0.05' where x = electrons from the event
# requires at least one electron that passes this selection
s1 = Selection(lambda x: x.hadronicOverEm() < 0.05, x = event.electrons, Selection.AtLeastOne)
result = s1.passes(event)
# same as s1 but selects on electron pt
s2 = Selection(lambda x: x.pt() > 30, x = event.electrons, Selection.AtLeastOne)
result = s2.passes(event)
# both selections at one
totalSelection = s1.then(s2)
result = totalSelection.passes(event)
# either selection
eitherSelection = s1 || s2
```