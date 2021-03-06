from ._discrete_distribution import DiscreteDistribution
from numpy import *


class IIDStdUniform(DiscreteDistribution):
    """
    A wrapper around NumPy's IID Standard Uniform generator `numpy.random.rand`.

    >>> dd = IIDStdUniform(dimension=2,seed=7)
    >>> dd.gen_samples(4)
    array([[0.076, 0.78 ],
           [0.438, 0.723],
           [0.978, 0.538],
           [0.501, 0.072]])
    >>> dd
    IIDStdUniform (DiscreteDistribution Object)
        d               2^(1)
        seed            7
        mimics          StdUniform
    """

    parameters = ['d','seed','mimics']

    def __init__(self, dimension=1, seed=None):
        """
        Args:
            dimension (int): dimension of samples
            seed (int): seed the random number generator for reproducibility
        """
        self.d = dimension
        self.seed = seed
        random.seed(self.seed)
        self.mimics = 'StdUniform'
        self.low_discrepancy = False
        super(IIDStdUniform,self).__init__()

    def gen_samples(self, n):
        """
        Generate samples 

        Args:
            n (int): Number of observations to generate

        Returns:
            ndarray: n x self.d array of samples
        """
        return random.rand(int(n), int(self.d))
    
    def pdf(self, x):
        return ones(x.shape[0], dtype=float)
    
    def _set_dimension(self, dimension):
        self.d = dimension
        