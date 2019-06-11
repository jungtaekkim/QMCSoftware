''' Originally developed in MATLAB by Fred Hickernell. Translated to python by Sou-Cheng T. Choi and Aleksei Sorokin '''
from numpy import array,zeros,std,mean,arange
from time import clock
from accumData import accumData


class meanVarData(accumData):
    '''
    Accumulated data for IIDDistribution calculations,
    stores the sample mean and variance of function values
    '''
    
    def __init__(self,nf):
        ''' nf = # functions '''
        super().__init__()
        self.muhat = zeros(nf) # sample mean
        self.sighat = zeros(nf) # sample standard deviation
        self.nSigma = zeros(nf) # number of samples used to compute the sample standard deviation
        self.nMu = zeros(nf)  # number of samples used to compute the sample mean

    def updateData(self, distribObj, funObj):
        for ii in range(len(funObj)):
            tStart = clock()  # time the function values
            dim = distribObj[ii].trueD.dimension
            distribData,*ignore = distribObj[ii].genDistrib(self.prevN[ii]+1,self.prevN[ii]+self.nextN[ii],self.nextN[ii],arange(1,dim+1))
            y = funObj[ii].f(distribData,arange(1,dim+1))
            self.costF[ii] = clock() - tStart  # to be used for multi-level methods
            if self.stage == 'sigma':
                self.sighat[ii] = std(y)  # compute the sample standard deviation if required
            self.muhat[ii] = y.mean(0)  # compute the sample mean
            self.solution = self.muhat.sum(0) # which also acts as our tentative solution
        return self

if __name__ == "__main__":
    # Doctests
    import doctest
    x = doctest.testfile("Tests/dt_meanVarData.py")
    print("\n"+str(x))
