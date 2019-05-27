from discreteDistribution import discreteDistribution
from randomstate.prng import mrg32k3a
from numpy import arange

class IIDDistribution(discreteDistribution):
    '''
    Specifies and generates the components of $\frac 1n \sum_{i=1}^n \delta_{\vx_i}(\cdot)$
    where the $\vx_i$ are IIDDistribution uniform on $[0,1]^d$ or IIDDistribution standard Gaussian
    '''

    def __init__(self,distribData=None,trueD=None):
        state = []
        super().__init__(distribData,state,trueD=trueD)
        if trueD:
            self.distrib_list = [IIDDistribution() for i in range(len(trueD))]
            # self now refers to self.distrib_list
            for i in range(len(self)):
                self[i].trueD = self.trueD[i]
                self[i].distribData = self.distribData[i] if self.distribData else None
        
    def initStreams(self):
        nObj = len(self)
        for ii in range(nObj):
            self[ii].stream = mrg32k3a.RandomState() # Throws warning
        return self

    def genDistrib(self,nStart,nEnd,n,coordIndex=None):
        if not coordIndex:
            coordIndex=arange(1,self.trueD.dimension+1)
        nPts = nEnd - nStart + 1  # how many points to be generated
        if self.trueDistribution=='stdUniform': # generate uniform points
            x = self.stream.rand(nPts,len(coordIndex))  # nodes
        elif self.trueD.measureName=='stdGaussian': # standard normal points
            x = self.stream.randn(nPts,len(coordIndex))  # nodes
        else:
            raise Exception('Distribution not recognized')
        return x,1,1/n

if __name__ == "__main__":
    import doctest
    x = doctest.testfile("Tests/dt_IIDDistribution.py")
    print("\n"+str(x))
