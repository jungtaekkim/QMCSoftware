from ._accumulate_data import AccumulateData
from numpy import *


class MLQMCData(AccumulateData):
    """
    Accumulated data for quasi-random sequence calculations,
    and store multi-level, multi-replications mean, variance, and cost values. 
    See the stopping criterion that utilize this object for references.
    """

    parameters = ['levels','dimensions','n_level','mean_level','var_level','bias_estimate','n_total']

    def __init__(self, stopping_crit, integrand, true_measure, discrete_distrib, n_init, replications):
        """
        Initialize data instance

        Args:
            stopping_crit (StoppingCriterion): a StoppingCriterion instance
            integrand (Integrand): an Integrand instance
            true_measure (TrueMeasure): A TrueMeasure instance
            discrete_distrib (DiscreteDistribution): a DiscreteDistribution instance
            replications (int): number of replications on each level
        """
        self.stopping_crit = stopping_crit
        self.integrand = integrand
        self.true_measure = true_measure
        self.discrete_distrib = discrete_distrib
        # Set Attributes
        self.levels = 2
        self.n_level = zeros(self.levels)
        self.dimensions = zeros(self.levels)
        self.eval_level = tile(True,self.levels)
        self.replications = replications
        self.n_init = n_init
        self.mean_level_reps = tile(0.,(self.levels,int(self.replications)))
        self.mean_level = tile(0.,self.levels)
        self.var_level = tile(inf,self.levels)
        self.cost_level = tile(inf,self.levels)
        self.bias_estimate = inf
        self.solution = None
        self.n_total = 0
        # get seeds for each replication
        random.seed(self.discrete_distrib.seed)
        self.seeds = random.randint(1, 1000000, size=(self.levels,int(self.replications)), dtype=uint64)
        super(MLQMCData,self).__init__()

    def update_data(self):
        """ See abstract method. """
        # update sample sums
        for l in range(self.levels):
            if not self.eval_level[l]:
                # nothing to do on this level
                continue
            # reset dimension
            self.dimensions[l] = self.integrand._dim_at_level(l)
            new_dim = int(self.dimensions[l])
            self.true_measure._set_dimension_r(new_dim)
            n_max = self.n_init if self.n_level[l]==0 else 2*self.n_level[l]
            for r in range(int(self.replications)):
                self.discrete_distrib.set_seed(self.seeds[l,r]) # reset seed
                samples = self.discrete_distrib.gen_samples(n_min=self.n_level[l],n_max=n_max)
                self.integrand.f(samples,l=l)
                prev_sum = self.mean_level_reps[l,r]*self.n_level[l]
                self.mean_level_reps[l,r] = (self.integrand.sums[0]+prev_sum)/float(n_max)
            self.n_level[l] = n_max
            self.mean_level[l] = self.mean_level_reps[l].mean()
            self.var_level[l] = self.mean_level_reps[l].var()
            self.cost_level[l] = self.var_level[l]/(self.dimensions[l]*self.n_level[l])
        self.bias_estimate = max(.5*abs(self.mean_level[self.levels-2]),\
                                 abs(self.mean_level[self.levels-1]))
        self.n_total = self.replications*self.n_level.sum()
        self.solution = self.mean_level.sum()
    
    def _add_level(self):
        """ Add another level to relevent attributes. """
        self.levels += 1
        self.dimensions = hstack((self.dimensions,0))
        self.n_level = hstack((self.n_level,0))
        self.eval_level = hstack((self.eval_level,True))
        self.mean_level_reps = vstack((self.mean_level_reps,zeros(int(self.replications))))
        self.mean_level = hstack((self.mean_level,0))
        self.var_level = hstack((self.var_level,inf))
        self.cost_level = hstack((self.cost_level,inf))
        self.seeds = vstack((self.seeds,random.randint(1, 1000000, int(self.replications), dtype=uint64)))
