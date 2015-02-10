#!/usr/bin/env python
import re
from pprint import pprint

d = dict()
with open("tmp1", "rt") as f:
  for l in f:
    v = re.match("\"(\\S+)\"\\,\\s+([^$]*)", l.strip());
    if v:
      k = v.group(1)
      v = v.group(2)
      d[k] = v
# pprint(d)

fo = open("tmp2", "wt")
with open("lpc4337fbd144", "rt") as f:
  for l in f:
    m = re.match("(\\s+)(\\d+)\\s*\\:\\s*\\[\\s*\\\"(\S+)\\\"\\s+\\],", l.rstrip())
    if m:
      tab = m.group(1)
      pnum = m.group(2)
      plabel = m.group(3)
      if plabel in d:
        st = "%s%s: [ \"%s\", %s ]," % (tab, pnum, plabel, d[plabel])
        print st
        continue
    print l.rstrip()
    #fo.write(l)
fo.close()
