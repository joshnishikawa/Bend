import ref

class Note(object):

    def __init__(self, tonic, entry):
        self.entry = entry
        self.name = ''.join(self.entry)
        self.value = ref.notes[self.entry[0]] + 12 * int(self.entry[1])
        
        # value is the number of semitones above C0
        # raw_val is the same number ignoring full octave intervals
        # deg_val is the number of semitones above the tonic
        raw_val = self.value % 12
        if raw_val > tonic:
            deg_val = raw_val - tonic
        elif raw_val < tonic:
            deg_val = raw_val + 12 - tonic
        else: deg_val = 0

        self.deg_val = deg_val
        self.degree = ref.degrees[self.deg_val]

        self.root = False # FIXME needs to be set at some point


class Interval(object):

    def __init__(self, newnote, othernote):
        self.notes = sorted([newnote, othernote], key = lambda x: x.value)
        self.bottom = self.notes[0].name
        self.top = self.notes[1].name
        self.value = self.notes[1].value - self.notes[0].value

        if self.value > 12:
            self.octaves, interval = divmod(self.value, 12)
        else: self.octaves, interval = 0, self.value

        # this slop just lets me use iterable tuples as keys in the chords dict 
        keylist = []
        keylist.append(interval)
        self.interval = tuple(keylist)
        
        self.name = ref.chords[(self.interval)][0]

class Chord(object):

    def __init__(self, newinterval, otherinterval):
        self.name = 'Bob'
