# Table of Contents
* [Selections](#selections)
* [Examples](#examples)
  * [Selecting an event by event number](#Selecting-an-event-by-event-number)
  * [Using your own selection function](#using-your-own-selection-function)
  * [Using collections](#using-collections)
  * [Using your own criterion function](#using-your-own-criterion-function)
  * [Merging selections](#merging-selections)
  * [Staging selections](#staging-selections)
  * [Accessing selection records (cut flows, selection efficiency, etc.)](#accessing-selection-records)


## Selections
Selections provide an easy way to specify how to select events. A selection consists of a name, one or more conditions (given as a selection function), an optional collection reference and an optional selection criterion function (at least one/two, etc, are provided by ```selection.Criteria.<criterion>```). The selection criterion is only evaluated for collections
The usage of the ```Selection``` class looks as follows:
```python
selection = Selection(<name>, <selection function>, <optional collection reference>, <optional criterion function>)
```
The ```selection``` instance can then be used to check if events would pass the selection:
```python
for event in events:
    if selection.selects(event):
        # do your thing
```

## Examples:
The examples assume that you either running over a ROOT tree or have data in a similar format. If you want to use the examples just by themselves, please add the following before the for-loop:
```python
Event = namedtuple('Event', ['run', 'number', 'electrons'])
Electron = namedtuple('Electron', ['pt', 'hadronicOverEm'])

e1 = Electron(50, 0.8)
e2 = Electron(30, 0.001)
e3 = Electron(23, 0.001)
electrons = [e1, e2, e3]

event1 = Event(1, 13700, electrons)
event2 = Event(2000, 64000, [e1, e3])
events = [event1, event2]
```

### Selecting an event by event number
```python
# select events with run > 2
s1 = Selection('Run range', lambda x: x.run > 1)
# selects events with event number < 2000 and run number == 1
s2 = Selection('Event number', lambda x: x.number < 20000 and x.run ==1 )

for event in events:
    r1 = s1.selects(event)
    r2 = s2.selects(event)
    ...
```

### Using your own selection function
In the above example we used the [lambda function](https://docs.python.org/2/tutorial/controlflow.html#lambda-expressions) which is a quick way to define to specify our requirements. However, you can also do more complicated things by defining your selection function:
```python
def func(x):
    result = x.run > 1
    result = result and x.number > 2000
    # etc

s1 = Selection('Run range', func) # <- now uses reference to your function
for event in events:
    r1 = s1.selects(event)
    ...
```

### Using collections
While plain types can be easily processed with the above, often you will need to process vectors of variables (e.g. electrons, jets, etc). This can be done by specifying the two optional parameters: the collection ```x``` and how many items you need to pass the selection (at least one/two, etc).
```python
pt_selection = Selection('pt cut', lambda x: x.pt >= 30, 'electrons', Criteria.AtLeastOne)
for event in events:
    result = pt_selection.selects(event)
    ...
```
The above selection will select events if it can find at least one item (```Criteria.AtLeastOne```) in the electron collection (```event.electrons```) with pt >= 30 GeV. So internally, ```x``` is replaced with items from ```event.electrons```.

### Using your own criterion function
The same way the selection function is constructed, one can specify a custom criterion function. The only difference is that the parameter is now the number of passed items from the collection:
```python
def exactly_one(n):
    return n == 1

pt_selection = Selection('pt cut', lambda x: x.pt >= 30, 'electrons', exactly_one)
for event in events:
    result = pt_selection.selects(event)
    ...
```
### Merging selections
For readability or other reasons it might be useful to combine multiple selections into one.
For this purpose the ```+``` operator is available:
```python
hoE_selection = Selection('H/E', lambda x: x.hadronicOverEm < 0.05, 'electrons', Criteria.AtLeastOne)
pt_selection = Selection('pt cut', lambda x: x.pt >= 30, 'electrons', Criteria.AtLeastOne)
hoE_and_pt = hoE_selection + pt_selection
for event in events:
    result = hoE_and_pt.selects(event)
    ...
```

Another useful feature is to take the OR between two selections. This is simply done by
```python
s1 = Selection('Run range', lambda x: x.run > 1)
s2 = Selection('Event number', lambda x: x.number < 20000 and x.run ==1 )
either_selection = s1 || s2
for event in events:
    result = either_selection.selects(event)
    ...
```

### Staging selections
Staging or chaining multiple selections is a very useful feature to construct a selection from multiple selection steps while retaining the information about each step (i.e. to construct a cut flow). This is done using the ```Selection.then``` class method.
```python
hoE_selection = Selection('H/E', lambda x: x.hadronicOverEm < 0.05, 'electrons', Criteria.AtLeastOne)
pt_selection = Selection('pt cut', lambda x: x.pt >= 30, 'electrons', Criteria.AtLeastOne)
hoE_and_pt = hoE_selection + pt_selection
for event in events:
    result = hoE_and_pt.selects(event)
    ...
cut_flow = hoE_and_pt.cutflow()
for i, cut in enumerate(cut_flow):
    # first item is the number of processed events
    print 'Step {}: {} events'.format(i, cut)
```

### Accessing selection records
An example of accessing the cut flow was shown in [Staging selections](#staging-selections). In addition to that the Selection class provides further access to internal counters:
```python
s1 = Selection('Run range', lambda x: x.run > 1)

for event in events:
    r1 = s1.selects(event)
    ...

# number of processed events
print(s1.n_processed())
# number of passed events
print(s1.n_passed())
# selection efficiency
print(s1.efficiency())
```
