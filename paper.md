---

title: 'Mishra-Warren Autodiff - A Generalized-Input Automatic Differentiation Library in C++'
tags:
  - automatic differentiation
  - algorithmic differentiation
  
authors:
  - name: Neil Warren^[Footnote description.]
    orcid: 0000-0001-7848-4226
    affiliation: 1
  - name: Aakash Mishra^[Footnote description.]
    orcid: 
    affiliation: 2
  - name: David Sondak^[Footnote description.]
    orcid: 
    affiliation: 3
  - name: Andrew Kirby^[Footnote description.]
    orcid: 
    affiliation: 4

affiliations:
 - name: Master's Student, Harvard University, Extension School
   index: 1
 - name: Undergraduate Student, Harvard College
   index: 2
- name: 
   index: 3
- name: 
   index: 4
 
date: 03 11 2021
bibliography: paper.bib
---

# Summary

In many modern scientific applications, from mathematical optimization to training neural networks, the need to efficiently compute derivatives is ubiquitous. Given the drawbacks using computer algebra systems to perform symbolic differentiation, which can become intractable for complex functions, automatic differentiation has become the preferred method of computing such derivatives. Several popular OSS machine learning libraries include automatic differentiation functionality: Pytorch [@paszke2017automatic], [@pytorch]; Tensorflow [@TFAutodiff].  While extensible, these libraries are limited in the scope of the input that they can process.  In particular, these libraries perform automatic differentiation on internal data types specific to the library, and therefore prescribe input be defined in the Python (or other) programming language by instantiation of these data types. Allowing input in a more general form, such as from scientific documents and text strings, allows for automation of what would otherwise be the manual task of inputting functions via source code declaration. 
 
# Statement of need

Mishra-Warren Autodiff is a C++ library that, first, applies Dijkstra's shunting yard algorithm to pre-process textual function inputs along with point vectors and derivative seed vectors, and then applies forward and reverse mode automatic differentiation algorithms to develop a well-formed Jacobian matrix. In contrast, most existing popular automatic differentiation libraries take functional input in the form of a computer program's source code (often Python) utilizing specialized data structures and previously declared variables and constants. [@TFAutodiff] This makes the process of applying automatic differentiation to functions from textual sources a manual task. Mishra-Warren Autodiff accepts raw textual input, e.g., "f(x0,y0) = x0 * cos(y0)," from the command line or a text file, such as a scientific document. 

# Acknowledgements

Mishra-Warren Autodiff was developed as an extension of a group project developed in the course "Systems Development for Computational Science," CS-107, at Harvard University in the Fall of 2020 under the intruction of Dr. David Sondak, Harvard Institute for Applied Computational Science, and Dr. Andrew Kirby, Post-doctoral associate at MIT Lincoln Laboratory, and in collaboration with Leo Landau and Samson Negassi. 

# References
