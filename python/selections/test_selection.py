from collections import namedtuple
from selection import Selection, Criteria
from nose2.tools.decorators import with_setup

Event = namedtuple('Event', ['run', 'number', 'electrons'])
Electron = namedtuple('Electron', ['pt', 'hadronicOverEm'])
e1, e2, e3, event1, event2 = None, None, None, None, None
hoE_selection, pt_selection = None, None
pt_selection_2, low_pt_selection = None, None


def setup():
    global e1, e2, e3, event1, event2, hoE_selection, pt_selection, pt_selection_2, low_pt_selection
    e1 = Electron(50, 0.8)
    e2 = Electron(30, 0.001)
    e3 = Electron(23, 0.001)
    electrons = [e1, e2, e3]
    event1 = Event(1, 13700, electrons)
    event2 = Event(2000, 64000, [e1, e3])

    hoE_selection = Selection(
        'H/E', lambda x: x.hadronicOverEm < 0.05, 'electrons', Criteria.AtLeastOne)
    pt_selection = Selection(
        'pt cut', lambda x: x.pt >= 30, 'electrons', Criteria.AtLeastOne)
    pt_selection_2 = Selection(
        'pt cut', lambda x: x.pt >= 30, 'electrons', Criteria.AtLeastTwo)
    low_pt_selection = Selection(
        'pt cut', lambda x: x.pt >= 20, 'electrons', Criteria.AtLeastOne)


@with_setup(setup)
def testSimpleCut():
    global event1, event2
    s1 = Selection('Run range', lambda x: x.run > 1)
    assert(not s1.selects(event1))
    assert(s1.selects(event2))

    s2 = Selection('Event number', lambda x: x.number < 20000)
    assert(s2.selects(event1))
    assert(not s2.selects(event2))


@with_setup(setup)
def testCollection():
    global event1, event2, hoe_selection, pt_selection_2

    assert(hoE_selection.selects(event1))
    assert(hoE_selection.selects(event2))

    assert(pt_selection_2.selects(event1))
    assert(not pt_selection_2.selects(event2))


@with_setup(setup)
def testSum():
    global event1, event2, hoe_selection, pt_selection

    s1 = hoE_selection + pt_selection

    assert(s1.selects(event1))
    assert(not s1.selects(event2))


@with_setup(setup)
def testChain():
    global event1, event2, hoe_selection, pt_selection

    s3 = hoE_selection.then(low_pt_selection)

    assert(s3.selects(event1))
    assert(s3.selects(event2))


@with_setup(setup)
def testSimpleCounter():
    global event1, event2, pt_selection

    s1 = Selection('Run range', lambda x: x.run > 1)

    pt_selection.selects(event1)
    pt_selection.selects(event1)
    s1.selects(event1)
    s1.selects(event2)

    assert(pt_selection.n_processed() == 2)
    assert(pt_selection.n_passed() == 2)
    assert(pt_selection.efficiency() == 1)
    assert(s1.n_processed() == 2)
    assert(s1.n_passed() == 1)
    assert(s1.efficiency() == 0.5)


@with_setup(setup)
def testSumCounter():
    global event1, event2, hoe_selection, pt_selection
    s1 = hoE_selection + pt_selection

    assert(s1.selects(event1))
    assert(not s1.selects(event2))
    assert(s1.n_processed() == 2)
    assert(s1.n_passed() == 1)
    assert(s1.efficiency() == 0.5)


@with_setup(setup)
def testChainCounter():
    global event1, event2, hoe_selection, pt_selection
 
    s1 = hoE_selection.then(low_pt_selection)
    s2 = Selection('Run range', lambda x: x.run > 1)
    s3 = hoE_selection.then(low_pt_selection).then(s2)
 
    assert(s1.selects(event1))
    assert(s1.selects(event2))
    assert(not s3.selects(event1))
    assert(s3.selects(event2))
     
    assert(s1.n_processed() == 2)
    assert(s1.n_passed() == 2)
    assert(s1.efficiency() == 1)
    assert(s3.n_processed() == 2)
    assert(s3.n_passed() == 1)
    assert(s3.efficiency() == 0.5)
    
