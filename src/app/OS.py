import platform

class Os: 
    systm = None
    version = None
    relaese =None

    @classmethod
    def initalizeOs(cls): 
        cls.system = platform.system()
        cls.version = platform.version()
        cls.release = platform.release()
