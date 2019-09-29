''' Originally developed in MATLAB by Fred Hickernell. Translated to python by Sou-Cheng T. Choi and Aleksei Sorokin '''
from numpy import random

from . import DiscreteDistribution

class IIDDistribution(DiscreteDistribution):
    '''
    Specifies and generates the components of $\frac 1n \sum_{i=1}^n \delta_{\vx_i}(\cdot)$
    where the $\vx_i$ are IIDDistribution uniform on $[0,1]^d$ or IIDDistribution standard Gaussian
    '''
    
    def __init__(self, trueD=None, distrib_data=None, rngSeed=None):
        accepted_measures = ['std_uniform','std_gaussian']
        if rngSeed: random.seed(rngSeed)
        super().__init__(accepted_measures, trueD, distrib_data)

    def gen_distrib(self, n, m, j=1):
        if self.trueD.measureName=='std_uniform': return random.rand(j,int(n),int(m)).squeeze()
        elif self.trueD.measureName=='std_gaussian': return random.randn(j,int(n),int(m)).squeeze()