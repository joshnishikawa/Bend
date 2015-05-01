import ref

class Key(object):

    scales = {
    'C major' : ['C', 'D', 'E', 'F', 'G', 'A', 'B'],
    'A minor' : ['A', 'B', 'C', 'D', 'E', 'F', 'G'],
    'G major' : ['G', 'A', 'B', 'C', 'D', 'E', 'F#'],
    'E minor' : ['E', 'F#', 'G', 'A', 'B', 'C', 'D'],
    'D major' : ['D', 'E', 'F#', 'G', 'A', 'B', 'C#'],
    'B minor' : ['B', 'C#', 'D', 'E', 'F#', 'G', 'A'],
    'A major' : ['A', 'B', 'C#', 'D', 'E', 'F#', 'G#'],
    'F# minor': ['F#', 'G#', 'A', 'B', 'C#', 'D', 'E'],
    'E major' : ['E', 'F#', 'G#', 'A', 'B', 'C#', 'D#'],
    'C# minor': ['C#', 'D#', 'E', 'F#', 'G#', 'A', 'B'],
    'B major' : ['B', 'C#', 'D#', 'E', 'F#', 'G#', 'A#'],
    'G# minor': ['G#', 'A#', 'B', 'C#', 'D#', 'E', 'F#'],
    'F# major': ['F#', 'G#', 'A#', 'B', 'C#', 'D#', 'E#'],
    'D# minor': ['D#', 'E#', 'F#', 'G#', 'A#', 'B', 'C#'],
    'C# major': ['C#', 'D#', 'E#', 'F#', 'G#', 'A#', 'B#'],
    'A# minor': ['A#', 'B#', 'C#', 'D#', 'E#', 'F#', 'G#'],
    'Cb major': ['Cb', 'Db', 'Eb', 'Fb', 'Gb', 'Ab', 'Bb'],
    'Ab minor': ['Ab', 'Bb', 'Cb', 'Db', 'Eb', 'Fb', 'Gb'],
    'Gb major': ['Gb', 'Ab', 'Bb', 'Cb', 'Db', 'Eb', 'F'],
    'Eb minor': ['Eb', 'F', 'Gb', 'Ab', 'Bb', 'Cb', 'Db'],
    'Db major': ['Db', 'Eb', 'F', 'Gb', 'Ab', 'Bb', 'C'],
    'Bb minor': ['Bb', 'C', 'Db', 'Eb', 'F', 'Gb', 'Ab'],
    'Ab major': ['Ab', 'Bb', 'C', 'Db', 'Eb', 'F', 'G'],
    'F minor' : ['F', 'G', 'Ab', 'Bb', 'C', 'Db', 'Eb'],
    'Eb major': ['Eb', 'F', 'G', 'Ab', 'Bb', 'C', 'D'],
    'C minor' : ['C', 'D', 'Eb', 'F', 'G', 'Ab', 'Bb'],
    'Bb major': ['Bb', 'C', 'D', 'Eb', 'F', 'G', 'A'],
    'G minor' : ['G', 'A', 'Bb', 'C', 'D', 'Eb', 'F'],
    'F major' : ['F', 'G', 'A', 'Bb', 'C', 'D', 'E'],
    'D minor' : ['D', 'E', 'F', 'G', 'A', 'Bb', 'C']
    }

    def __init__(self, entry):
        self.value = ref.notes[entry[0]]
        self.mode  = entry[1]
        self.name  = ' '.join(entry)
        self.scale = Key.scales[self.name]
        self.tonic = self.scale[0]
        self.supt  = self.scale[1]
        self.med   = self.scale[2]
        self.subd  = self.scale[3]
        self.dom   = self.scale[4]
        self.subm  = self.scale[5]
        self.subt  = self.scale[6]


class Note(object):

    degrees = ['the Tonic', 'the Super Tonic', 'the Mediant',
               'the Sub Dominant', 'the Dominant', 'the Sub Mediant',
               'the Sub Tonic']

    def __init__(self, tonic, entry, scale):
        
        self.name = entry[0]
        self.range = int(entry[1])
        self.value = ref.notes[entry[0]] + 12 * self.range
        
        # value is the number of semitones above C0
        # deg_val is the number of semitones above the tonic
        if ref.notes[entry[0]] > tonic:
            self.deg_val = ref.notes[entry[0]] - tonic
        elif self.value % 12 < tonic:
            self.deg_val = ref.notes[entry[0]] + 12 - tonic
        else: self.deg_val = 0
        
        if entry[0] in scale:
            self.degree = Note.degrees[scale.index(entry[0])]
        else: self.degree = 'not in the diatonic scale'
        

class Chord(object):

    roots ={'default': 0,
        (0,): 0,
        (0, 1): 1,
        (0, 2): 2,
        (0, 3): 0,
        (0, 4): 0,
        (0, 5): 5,
        (0, 6): 0,
        (0, 7): 0,
        (0, 8): 8,
        (0, 9): 9,
        (0, 10): 0,
        (0, 11): 0,
        (0, 4, 7): 0,
        (0, 7, 12): 0,
        (0, 3, 8): 8,
        (0, 5, 9): 5,
        (0, 3, 7): 0,
        (0, 4, 9): 9,
        (0, 5, 8): 5,
        (0, 3, 6): 0,
        (0, 3, 8): 8,
        (0, 6, 9): 6,
        (0, 4, 8): 0, 
        (0, 2, 7): 0,
        (0, 5, 10): 10,
        (0, 5, 7): 5,
        (0, 4, 5): 0,
        (0, 1, 3): 3,
        (0, 7, 11): 7,
        (0, 4, 10): 0,
        (0, 4, 11): 0,
        (0, 7, 8): 8,
        (0, 1, 5): 1,
        (0, 3, 10): 0,
        (0, 7, 9): 9,
        (0, 2, 5): 2,
        (0, 7, 10): 0,
        (0, 3, 8): 8,
        (0, 2, 6): 2,
        (0, 3, 11): 0,
        (0, 8, 9): 9,
        (0, 1, 4): 1,
        (0, 4, 7, 11): 0,
        (0, 3, 7, 8): 8,
        (0, 4, 5, 9): 5,
        (0, 1, 5, 8): 1,
        (0, 3, 7, 10): 0,
        (0, 4, 7, 9): 9,
        (0, 3, 5, 8): 5,
        (0, 2, 5, 9): 2,
        (0, 4, 7, 10): 0,
        (0, 3, 6, 8): 8,
        (0, 3, 5, 9): 5,
        (0, 2, 6, 9): 2,
        (0, 3, 6, 9): 0,
        (0, 3, 6, 10): 0,
        (0, 3, 7, 9): 9,
        (0, 4, 6, 9): 6,
        (0, 2, 5, 8): 2,
        (0, 3, 7, 11): 0,
        (0, 4, 8, 9): 9,
        (0, 4, 5, 8): 5,
        (0, 1, 4, 8): 1,
        (0, 4, 8, 10): 0,
        (0, 4, 6, 8): 8,
        (0, 2, 4, 8): 4,
        (0, 2, 6, 10): 2,
        (0, 4, 8, 11): 0,
        (0, 4, 7, 8): 8,
        (0, 3, 4, 8): 4,
        (0, 1, 5, 9): 1,
        (0, 4, 6, 10): 0,
        (0, 2, 4, 7, 11): 0,
        (0, 2, 7, 11): 0,
        (0, 3, 4, 10): 0,
        (0, 2, 3, 6, 10): 0,
        (0, 2, 4, 7, 9): 0,
        (0, 6, 7): 0
        }
    
    inversions = [
        'in root position',
        'in 1st inversion',
        'in 2nd inversion',
        'in 3rd inversion',
        'in 4th inversion',
        'in 5th inversion',
        'in 6th inversion'
        ]

    def __init__(self, mode, notes):

        intervals = [0]
        for n in notes[1:]:
            interval = (n.value - notes[0].value) % 12
            intervals.append(interval)
        intervals.sort()
        self.quality = tuple(intervals) # list can't be used as dict key
        
        self.bass = notes[0].deg_val
        
        if self.quality in Chord.roots:
            self.root = (Chord.roots[self.quality] + self.bass) % 12
        else: self.root = 0
        
        if len(self.quality) == 2:
            self.name = ref.chords[(self.quality)]['default']
            self.inversion = Chord.inversions[0]
        else:
            for q in ref.chords.keys():
                inv = 0
                found = False
                for inversion in q:
                    if inversion == self.quality:
                        found = True
                        if self.root in ref.chords[q] and \
                                    mode in ref.chords[q][self.root]:
                            self.name=ref.chords[q][self.root][mode]
                        else: self.name = ref.chords[q]['default']
                        self.inversion = Chord.inversions[inv]
                        break
                    else: inv += 1
                if found: break
                else: pass
            else:
                self.name = ref.chords['default']
                self.inversion = ''
