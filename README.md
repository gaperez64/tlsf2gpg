This is a translator from TLSF specifications to one of the following:
1. Parity games played on graphs (not on automata!) in the PGSolver format
2. Generalized parity games played on graphs in an extended PGSolver format
3. Parity automata in an extended HOA format

# Formats
Check out https://github.com/tcsprojects/pgsolver for details on the PGSolver
format and https://arxiv.org/abs/1912.05793 for the extended HOA format.

# Dependencies
* Syfco (the hacked version available here https://github.com/gaperez64/syfco)
* Spot's ltl2tgba
* cpphoafparser (the required sources are included here)
* CUDD 2.5.1 (a compressed copy is enclosed)

# Citing
If you use this software for your academic work, please cite the report
regarding the extended HOA format. For the translation to PGSolver format,
please cite our _Reachability Problems_ paper on generalized parity games.

```
@article{DBLP:journals/corr/abs-1912-05793,
  author    = {Guillermo A. P{\'{e}}rez},
  title     = {The Extended {HOA} Format for Synthesis},
  journal   = {CoRR},
  volume    = {abs/1912.05793},
  year      = {2019},
  url       = {http://arxiv.org/abs/1912.05793},
  archivePrefix = {arXiv},
  eprint    = {1912.05793},
  timestamp = {Tue, 03 Mar 2020 16:02:55 +0100},
  biburl    = {https://dblp.org/rec/journals/corr/abs-1912-05793.bib},
  bibsource = {dblp computer science bibliography, https://dblp.org}
}

@inproceedings{DBLP:conf/rp/BruyerePRT19,
  author    = {V{\'{e}}ronique Bruy{\`{e}}re and
               Guillermo A. P{\'{e}}rez and
               Jean{-}Fran{\c{c}}ois Raskin and
               Cl{\'{e}}ment Tamines},
  editor    = {Emmanuel Filiot and
               Rapha{\"{e}}l M. Jungers and
               Igor Potapov},
  title     = {Partial Solvers for Generalized Parity Games},
  booktitle = {Reachability Problems - 13th International Conference, {RP} 2019,
               Brussels, Belgium, September 11-13, 2019, Proceedings},
  series    = {Lecture Notes in Computer Science},
  volume    = {11674},
  pages     = {63--78},
  publisher = {Springer},
  year      = {2019},
  url       = {https://doi.org/10.1007/978-3-030-30806-3\_6},
  doi       = {10.1007/978-3-030-30806-3\_6},
  timestamp = {Mon, 09 Sep 2019 15:37:02 +0200},
  biburl    = {https://dblp.org/rec/conf/rp/BruyerePRT19.bib},
  bibsource = {dblp computer science bibliography, https://dblp.org}
}
```

# License
Licenses for cpphoafparser and CUDD are included. The new code is licensed
under GNU LGPL, just like cpphoafparser.
