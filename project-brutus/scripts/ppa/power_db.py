import logging
from sqlalchemy import *
from sqlalchemy.orm import *
from sqlalchemy.ext.declarative import declarative_base

engine = create_engine('sqlite:///power/power.db')
Base = declarative_base()
Session = sessionmaker(bind=engine)

def get_session():
    return Session()

def drop_table(db_name):
    logging.info('dropping table %s' % (db_name,))
    session = get_session()
    session.query(db[db_name]).delete()
    session.flush()
    session.commit()

def find_one(db_name, fields={}):
    session = get_session()
    row = session.query(db[db_name]).filter_by(**fields).first()
    if row is None:
        return None
    # return {c.name: getattr(row, c.name) for c in row.__table__.columns}
    return row.__dict__
    
def find(db_name, fields={}):
    session = get_session()
    return session.query(db[db_name]).filter_by(**fields)

def insert(db_name, **kwargs):
    session = get_session()

    obj = db[db_name](**kwargs)
    session.add(obj)
    session.commit()

def initialize():
    for db in ['Fetch1Logic',
               'Fetch2Logic',
               'DecodeLogic',
               'RenameLogic',
               'DispatchLogic',
               'IssueLogic',
               'RegReadLogic',
               'LSULogic',
               'ExecuteMLogic',
               'ExecuteCtrlLogic',
               'ExecuteSLogic',
               'ExecuteCLogic',
               'ExecuteSCLogic',
               'RetireLogic']:
        populate_table(db)

def populate_table(name):
    logging.basicConfig(level=logging.DEBUG,
                        format='[%(levelname)s] %(funcName)s :: %(message)s')
    session = get_session()

    with open('power/%s.csv' % (name, ), 'r') as fp:
        # header line contains the keys
        keys = fp.readline().rstrip().split(',')
        for line in fp:
            # split the csv line
            values = line.rstrip().split(',')
            d = dict(zip(keys, values))
            if 'RAMs_in_path' in d:
                del(d['RAMs_in_path'])
            logging.debug('Adding to %s: %s' %(name, str(d)))
            n = db[name](tech=45, **d)
            session.add(n)
        session.commit()
    
def print_item(item):
    if item is None:
        print ''
        return

    s = ''
    for (key, value) in item.__dict__.items():
        s+= str(key) + ': ' + str(value) + '\n'
    print s


def add_row(db_name, fields, allow_duplicate=False):
    '''
    adds a new row to a database.

    db_name: database to add a new row to
    fields: dict describing the columns and values of the new row
    allow_duplicate: when False, a new row won't be added
        if there's already a row described by fields

    return: None
    '''

    session = get_session()

    # check for duplicates
    if not allow_duplicate:
        r = find_one(db_name, fields)
        if r is not None:
            return 

    row = db[db_name](**fields)
    session.add(row)
    session.commit()


def required_knobs(db_name):
    '''
    returns a list of the required knobs for a lookup into db_name
    '''
    return db[db_name].required_knobs

class Fetch1Logic(Base):
    __tablename__       = 'Fetch1Logic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width']

    def __init__(self, tech, fetch_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class Fetch2Logic(Base):
    __tablename__       = 'Fetch2Logic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width']

    def __init__(self, tech, fetch_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class DecodeLogic(Base):
    __tablename__       = 'DecodeLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width']

    def __init__(self, tech, fetch_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class RenameLogic(Base):
    __tablename__       = 'RenameLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width']

    def __init__(self, tech, fetch_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class DispatchLogic(Base):
    __tablename__       = 'DispatchLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    issue_width         = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width', 'issue_width']

    def __init__(self, tech, fetch_width, issue_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.issue_width        = int(issue_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class IssueLogic(Base):
    __tablename__       = 'IssueLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    issue_width         = Column(Integer)
    size_iq             = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width', 'issue_width', 'size_iq']

    def __init__(self, tech, fetch_width, issue_width, size_iq, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.issue_width        = int(issue_width)
        self.size_iq            = int(size_iq)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class RegReadLogic(Base):
    __tablename__       = 'RegReadLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    issue_width         = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['issue_width']

    def __init__(self, tech, issue_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.issue_width        = int(issue_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class LSULogic(Base):
    __tablename__       = 'LSULogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    issue_width         = Column(Integer)
    retire_width        = Column(Integer)
    size_lsq            = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['issue_width', 'retire_width', 'size_lsq']

    def __init__(self, tech, issue_width, retire_width, size_lsq, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.issue_width        = int(issue_width)
        self.retire_width       = int(retire_width)
        self.size_lsq           = int(size_lsq)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class ExecuteMLogic(Base):
    __tablename__       = 'ExecuteMLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = []

    def __init__(self, tech, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class ExecuteCtrlLogic(Base):
    __tablename__       = 'ExecuteCtrlLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = []

    def __init__(self, tech, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class ExecuteSLogic(Base):
    __tablename__       = 'ExecuteSLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = []

    def __init__(self, tech, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class ExecuteCLogic(Base):
    __tablename__       = 'ExecuteCLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    depth               = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['depth']

    def __init__(self, tech, depth, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.depth              = int(depth)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class ExecuteSCLogic(Base):
    __tablename__       = 'ExecuteSCLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    depth               = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['depth']

    def __init__(self, tech, depth, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.depth              = int(depth)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class RetireLogic(Base):
    __tablename__ = 'RetireLogic'
    id                  = Column(Integer, primary_key=True)
    tech                = Column(Integer)
    fetch_width         = Column(Integer)
    issue_width         = Column(Integer)
    retire_width        = Column(Integer)
    area_mm2            = Column(Float)
    delay_ns            = Column(Float)
    switching_power_mw  = Column(Float)
    leakage_power_mw    = Column(Float)
    required_knobs      = ['fetch_width', 'issue_width', 'retire_width']

    def __init__(self, tech, fetch_width, issue_width, retire_width, area_mm2, delay_ns, switching_power_mw, leakage_power_mw):
        self.tech               = int(tech)
        self.fetch_width        = int(fetch_width)
        self.issue_width        = int(issue_width)
        self.retire_width       = int(retire_width)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.switching_power_mw = float(switching_power_mw)
        self.leakage_power_mw   = float(leakage_power_mw)


class SRAM(Base):
    __tablename__    = 'SRAM'
    id               = Column(Integer, primary_key=True)
    tech             = Column(Integer)
    words            = Column(Integer)
    width            = Column(Integer)
    read_ports       = Column(Integer)
    write_ports      = Column(Integer)
    area_mm2         = Column(Float)
    delay_ns         = Column(Float)
    energy_per_read_nj  = Column(Float)
    energy_per_write_nj = Column(Float)
    leakage_power_mw = Column(Float)

    def __init__(self, tech, words, width, read_ports, write_ports, area_mm2, delay_ns, energy_per_read_nj, energy_per_write_nj, leakage_power_mw):
        self.tech               = int(tech)
        self.words              = int(words)
        self.width              = int(width)
        self.read_ports         = int(read_ports)
        self.write_ports        = int(write_ports)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.energy_per_read_nj    = float(energy_per_read_nj)
        self.energy_per_write_nj   = float(energy_per_write_nj)
        self.leakage_power_mw   = float(leakage_power_mw)


class CAM(Base):
    __tablename__    = 'CAM'
    id               = Column(Integer, primary_key=True)
    tech             = Column(Integer)
    words            = Column(Integer)
    width            = Column(Integer)
    read_ports       = Column(Integer)
    write_ports      = Column(Integer)
    area_mm2         = Column(Float)
    delay_ns         = Column(Float)
    energy_per_read_nj  = Column(Float)
    energy_per_write_nj = Column(Float)
    leakage_power_mw = Column(Float)

    def __init__(self, tech, words, width, read_ports, write_ports, area_mm2, delay_ns, energy_per_read_nj, energy_per_write_nj, leakage_power_mw):
        self.tech               = int(tech)
        self.words              = int(words)
        self.width              = int(width)
        self.read_ports         = int(read_ports)
        self.write_ports        = int(write_ports)
        self.area_mm2           = float(area_mm2)
        self.delay_ns           = float(delay_ns)
        self.energy_per_read_nj    = float(energy_per_read_nj)
        self.energy_per_write_nj   = float(energy_per_write_nj)
        self.leakage_power_mw   = float(leakage_power_mw)


# create tables
Base.metadata.create_all(engine)

# add all classes here so modules can access them with a string
db = {}
db['Fetch1Logic']       = Fetch1Logic
db['Fetch2Logic']       = Fetch2Logic
db['DecodeLogic']       = DecodeLogic
db['RenameLogic']       = RenameLogic
db['DispatchLogic']     = DispatchLogic
db['IssueLogic']        = IssueLogic
db['RegReadLogic']      = RegReadLogic
db['LSULogic']          = LSULogic
db['ExecuteMLogic']     = ExecuteMLogic
db['ExecuteCtrlLogic']  = ExecuteCtrlLogic
db['ExecuteSLogic']     = ExecuteSLogic
db['ExecuteCLogic']     = ExecuteCLogic
db['ExecuteSCLogic']    = ExecuteSCLogic
db['RetireLogic']       = RetireLogic
db['SRAM']              = SRAM
db['CAM']               = CAM
