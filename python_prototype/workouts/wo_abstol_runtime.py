#!/usr/bin/python_prototype/
from time import time
from matplotlib import pyplot as mpl_plot
from numpy import arange, array
import numpy as np
import pandas as pd

from qmcpy import integrate
from qmcpy.measures import StdUniform,StdGaussian,BrownianMotion,Lattice,Sobol
from qmcpy.distribution import IIDDistribution,QuasiRandom
from qmcpy.integrand import AsianCall
from qmcpy.stop import CLT,CLTRep

def plot(title,xlabel,ylabel,xdata,ydata,outF):
    #mpl_plot.title(title)
    mpl_plot.xlabel(xlabel,fontsize=14)
    mpl_plot.ylabel(ylabel,fontsize=14)
    for name,(trend,color) in ydata.items():
        mpl_plot.loglog(xdata,trend,basex=10,basey=10,color=color,label=name)
    #mpl_plot.xticks([5*10**-2,10**-1],fontsize=12)
    #mpl_plot.yticks([0,10,20,30],fontsize=12)
    mpl_plot.legend(
        loc = 'lower left',
        bbox_to_anchor = (0.0, 1.01),
        ncol = 2,
        borderaxespad = 0,
        frameon = False,
        prop = {'size': 14})
    mpl_plot.savefig(outF+'.png',
        dpi=500,
        bbox_inches = 'tight',
        pad_inches = .05)
    mpl_plot.show(block=False)

def QMC_Wrapper(stopObj,distribObj,name):
    item_f = '    %-25s %-10.3f %-10.3f'
    item_s = '    %-25s %-10s %-10s'
    #try:
    measureObj = BrownianMotion(
        time_vector=[arange(1/4,5/4,1/4),arange(1/16,17/16,1/16),arange(1/64,65/64,1/64)])
    OptionObj = AsianCall(measureObj)
    sol,dataObj = integrate(OptionObj,measureObj,distribObj,stopObj)
    print(item_f%(name,sol,dataObj.t_total))
    return sol,dataObj.t_total
    #except:
    #    print(item_s%(name,'',''))
    #    return '',''

def comp_Clt_vs_cltRep_runtimes(abstols):

    df_metrics = pd.DataFrame({'abs_tol':[],
        'CLT_stdUniform_sol':[],     'CLT_stdUniform_runTime':[],
        'CLT_stdGaussian_sol':[],    'CLT_stdGaussian_runTime':[],
        'CLT_Rep_lattice_sol':[],    'CLT_Rep_lattice_runTime':[],
        'CLT_Rep_Sobol_sol':[],      'CLT_Rep_Sobol_runTime':[]})
    for i,abs_tol in enumerate(abstols):
        print('abs_tol: %-10.3f'%abs_tol)
        results = [] # hold row of DataFrame
        results.append(abs_tol)

        # CLT_stdUniform
        distribObj = IIDDistribution(true_distribution=StdUniform(dimension=[4, 16, 64]), seed_rng=7)
        stopObj = CLT(distribObj,abs_tol=abs_tol)
        mu,t = QMC_Wrapper(stopObj,distribObj,'CLT_stdUniform')
        results.extend([mu,t])

        # CLT_stdGaussian
        distribObj = IIDDistribution(true_distribution=StdGaussian(dimension=[4, 16, 64]), seed_rng=7)
        stopObj = CLT(distribObj,abs_tol=abs_tol)
        mu,t = QMC_Wrapper(stopObj,distribObj,'CLT_stdGaussian')
        results.extend([mu,t])

        # CLT_Rep_lattice
        distribObj = QuasiRandom(true_distribution=Lattice(dimension=[4,16,64]),seed_rng=7)
        stopObj = CLTRep(distribObj,n_max=2**20,abs_tol=abs_tol)
        mu,t = QMC_Wrapper(stopObj,distribObj,'lattice')
        results.extend([mu,t])

        # CLT_Rep_sobol
        distribObj = QuasiRandom(true_distribution=Sobol(dimension=[4, 16, 64]), seed_rng=7)
        stopObj = CLTRep(distribObj,n_max=2**20,abs_tol=abs_tol)
        mu,t = QMC_Wrapper(stopObj,distribObj,'sobol')
        results.extend([mu,t])

        df_metrics.loc[i] = results # update metrics
    return df_metrics

if __name__ == '__main__':
    outF = 'outputs/Compare_true_distribution_and_StoppingCriterion_vs_Abstol'
    # Run Test

    absTols = arange(.01,.051,.002) # arange(.01,.06,.01)
    df_metrics = comp_Clt_vs_cltRep_runtimes(absTols)
    df_metrics.to_csv(outF+'.csv',index=False)

    # Gen Plot
    df = pd.read_csv(outF+'.csv')
    plot(title = 'Integration Time by Absolute Tolerance \nfor Multi-level Asian Option Function',
        xlabel = 'Absolute Tolerance',
        ylabel = 'Integration Runtime',
        xdata = df['abs_tol'].values,
        ydata = {
            'CLT: IID Gaussian':(df['CLT_stdUniform_runTime'],'r'),
            'CLT: IID Uniform ':(df['CLT_stdGaussian_runTime'],'b'),
            'CLT Repeated: Lattice':(df['CLT_Rep_lattice_runTime'],'g'),
            'CLT Repeated: sobol':(df['CLT_Rep_Sobol_runTime'],'y')},
        outF = outF)