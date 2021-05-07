--- 
affiliations: 
  - 
    index: 1
    name: "Harvard University, Division of Continuing Education"
  - 
    index: 2
    name: "Harvard University, Dept. of Computer Science"
  - 
    index: 3
    name: "Harvard University, Institute for Applied Computational Science"
  - 
    index: 4
    name: "Scientific Simulations, LLC."
authors: 
  - 
    affiliation: 1
    name: "Neil Warren^[Footnote description.]"
    orcid: 0000-0001-7848-4226
  - 
    affiliation: 2
    name: "Aakash Mishra^[Footnote description.]"
    orcid: ~
  - 
    affiliation: 3
    name: "David Sondak^[Footnote description.]"
    orcid: ~
  - 
    affiliation: 4
    name: "Andrew Kirby^[Footnote description.]"
    orcid: ~
bibliography: paper.bib
date: "03 11 2021"
tags: 
  - "automatic differentiation"
  - "algorithmic differentiation"
title: "GenAD: A Generalized Input Automatic Differentiation Library in C++"

---

# Summary

From mathematical optimization to neural network training, the need to efficiently compute derivatives of
functions containing tens or even thousands of variables and operations has become ubiquitous in scientific computing
[**reference!**] and scientific machine learning [@JMLR:v18:17-468]. Given the computational drawbacks of performing symbolic
differentiation, which quickly becomes intractable under moderate functional complexity [**reference to computational
complexity of symbolic differentiation**] for even the most sophisticated computer algebra systems[**cite some of the
systems**], automatic differentiation has emerged as a powerful method to compute derivatives. The emerging field of
differential programming [**references**] provides substantial support for writing differential scientific programs. Indeed,
modern programming languages, such as the Julia Programming Language [**cite!**], natively support automatic differentiation.
Within the machine learning community, several popular open source libraries include automatic differentiation functionality
including Pytorch [@paszke2017automatic], [@pytorch] and Tensorflow [@TFAutodiff]. While powerful and extensible, these
libraries are limited in the scope and form of the input that they can accept. In particular, users of these libraries leverage
computer source code (often Python) to input the function to be differentiated or write source code to derive the internal
representation of the functions. For functions derived outside of the computational environment, definition and data input 
is a largely manual process.  

The GenAD library presented in this work utilizes forms of generalized input (e.g., non-specific to the computational
environment), such as scientific documents and text strings. Acceptance of more generalized forms of input allows for the
efficient automation of automatic differentiation for functions derived in varied computational environments. This is
particularly useful for large functions of tens or thousands of variables, for example, as provided by output in text
form from documents or as output from libraries written in other languages with incompatible data types. 

# Statement of need

In many applications, scientists may use a software library that lacks built-in efficient automatic differentiation
functionality.<!--to a generate complex function with many variables and need to efficiently calculate the derivative or a
series of derivatives of the function. I DIDN'T UNDERSTAND THE LAST CLAUSE [DLS]--> For functions involving a large number of
variables (e.g. hundreds or thousands), the data input process for providing that function to a standard machine learning
library such as Pytorch or Tensorflow can be exceedingly time consuming and prone to error. Thus, there is a need for an
automatic differentiation library that can read in the function to be evaluated in a generalized form, such as raw text,
differentiate it to machine precision, and easily write out the result to a file by an external software library [DLS: I
don't understand thie last part starting at easily. Let's discuss]. 

GenAD is a C++ library that applies Dijkstra's shunting yard algorithm [@boysen2012shunting] to pre-process textual function
inputs along with point vectors and derivative seed vectors, and then applies forward and reverse mode automatic
differentiation algorithms to develop a well-formed Jacobian matrix [*The previous sentence is way too long. Split it into 2
or 3 sentences.*]. In contrast, most existing popular automatic differentiation libraries take functional input in the form
of a computer program's source code (often Python) utilizing specialized data structures and previously declared variables
and constants [@TFAutodiff]. This makes the process of applying automatic differentiation to functions from textual sources a
manual task. GenAD accepts raw textual input, e.g., "f(x0,y0) = x0 * cos(y0)," from the command line or a text file, such as
a scientific document. 

# Features 

GenAD utilizes functional descriptions as input and provides a complete well-formed Jacobian matrix as output. The user may
optionally select either forward mode or reverse mode automatic differentiation, the relative advantages and disadvantages of
which have been discussed at length in the literature [@DBLP:journals/corr/abs-1811-05031] [@JMLR:v18:17-468].

As shown in \autoref{fig:Parsing}, the GenAD shunting yard pre-processing implementation achieves approximately O(n) time
complexity over the number of generated tokens (approximately proportional to the number of elementary functions in the input
function). This was confirmed using a standard laptop with an Intel® Core™ i7-8550U CPU running 100 averaged trials with
functions of up to 524,288 elementary operations and independent variables. Independent of the functional complexity, given
that variable names given in text (e.g., “x0,” "x1," ... “x524287”) grow in length, parsing text for variable names in the
tokenizer causes the shape of the curve to become more quadratic as the input size increases. 

![ Parsing tokenization scaling.\label{fig:Parsing}](processing.png)

Given the algorithmic constraints of forward mode automatic differentiation, which computes partial derivatives for each
independent variable at each computational node, reverse mode provides for more efficient computation of derivatives for
functions with a large numbers of independent variables. Reverse mode, in contrast, requires the computation of an adjoint
tree from a first pass through the function tokens, which adds a small amount of computational overhead and thus is not
optimal for functions of only a few variables. Figure \autoref{fig:Parsing} shows the computational time complexity for the parsing and
evaluation of the Jacobian for functions of 1 to $2^{18}$ variables. 

![ Forward vs. Reverse Mode.\label{fig:fwdrev }](fwd_vs_rev.png)

# Acknowledgements

GenAD was developed as an extension of a group project developed in the course "Systems Development for Computational
Science," CS-107, at Harvard University in the Fall of 2020 under the instruction of Dr. David Sondak, Harvard Institute for
Applied Computational Science, and Dr. Andrew Kirby, Research Scientist at Scientific Simulations, LLC., and in collaboration
with Leo Landau and Samson Negassi. 

# References

