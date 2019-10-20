"""
Generating QMCPy's HTML documentation.
"""
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
# import sphinx_rtd_theme
# from sphinx.ext.napoleon.docstring import GoogleDocstring
# import sphinx_bootstrap_theme

sys.path.insert(0, os.path.abspath('.'))
sys.path.insert(0, os.path.abspath('../'))  # python_prototype
sys.path.insert(0, os.path.abspath('../python_prototype/'))
print("root directory = %s " % os.getcwd())

# -- Sphinx with Markdown ----------------------------------------------------

import recommonmark
from recommonmark.transform import AutoStructify
from recommonmark.parser import CommonMarkParser
import sphinx_bootstrap_theme

# -- Project information -----------------------------------------------------
project = u'qmcpy'
copyright = u'2019, Illinois Institute of Technology'
author = u'Fred J. Hickernell, Aleksei Sorokin, Sou-Cheng T. Choi'

# The full version, including alpha/beta/rc tags
release = u'0.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.

extensions = ['sphinx.ext.autodoc',
              'sphinx.ext.githubpages',
              # 'sphinx.ext.imgmath',
              'sphinx.ext.napoleon',
              'sphinx.ext.graphviz',
              'sphinx.ext.intersphinx',
              'sphinx_rtd_theme',
              'm2r',
              'sphinx.ext.mathjax',
              'sphinx.ext.doctest',
              'sphinx.ext.coverage',
              'sphinx.ext.ifconfig',
              'sphinx.ext.githubpages',
              'sphinx_markdown_tables']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# Napoleon settings
napoleon_google_docstring = True
napoleon_numpy_docstring = False
napoleon_include_init_with_doc = True
napoleon_include_private_with_doc = False
napoleon_include_special_with_doc = False
napoleon_use_admonition_for_examples = False
napoleon_use_admonition_for_notes = False
napoleon_use_admonition_for_references = False
napoleon_use_ivar = False
napoleon_use_param = True
napoleon_use_rtype = True
napoleon_use_keyword = True

# intersphinx
intersphinx_mapping = {
    'python': ('https://docs.python.org/3.7/', None),
    'numpy': ('https://docs.scipy.org/doc/numpy', None),
    'scipy': ('https://docs.scipy.org/doc/scipy/reference', None)
}

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#

# Reference: Sphinx Themes, https://tinyurl.com/y3nwsml4

"""
# html_theme = 'alabaster'
# Fork me on GitHub banner
html_theme_options = {
    'font_size': '18px',
    'github_user': 'QMCSoftware',
    'github_repo': 'QMCSoftware',
    'github_button': True,
    'github_banner': True
}
"""

html_theme = "sphinx_rtd_theme"
html_show_sourcelink = False

"""
html_theme = 'bootstrap'
html_theme_path = sphinx_bootstrap_theme.get_html_theme_path()
"""

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
#html_static_path = ['_static']

# -- Options for LaTeX output ---------------------------------------------

# Paths  on S.C. Choi's Mac machine for latex
#imgmath_latex = r"/Library/TeX/texbin/latex"
#imgmath_dvipng = r"/Library/TeX/texbin/dvipng"
#imgmath_font_size = 10


mathjax_config = {
    'extensions': ['tex2jax.js'],
    'jax': ['input/TeX', 'output/HTML-CSS'],
}


# -- Sphinx with Markdown ----------------------------------------------------

def setup(app):
    app.add_config_value('recommonmark_config', {
        'enable_math': True,
        'enable_eval_rst': True,
        'auto_code_block': True,
        }, True)
    app.add_transform(AutoStructify)
