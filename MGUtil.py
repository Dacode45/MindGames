class MGHeadSetError(Exception):
    def __init__(self, message, errors):

        #Call base class constructor
        super(ValidationError, self).__init__(message)

        #now custom error code
        self.errors = errors
