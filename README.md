# DatalogReader

Code reads Datalog input and uses it to analyze the queries.

Using input of the following format
  Schemes:
  Parent(p,c)
  Sibling(a,b)
  Ancestor(x,y)

  Facts:
  Parent('bob','ned').
  Parent('jim','bob').
  Sibling('sue','ned').

  Rules:
  Sibling(x,y):-Sibling(y,x).
  Ancestor(x,y):-Ancestor(x,z),Parent(z,y).
  Ancestor(x,y):-Parent(x,y).

  Queries:
  Ancestor(x,'ned')?
  Sibling('ned','sue')?
  
It first, reads through the input, parsing the schemes, facts, rules and queries into their
own vectors. Then it creates a realtion based off of the scemes as heads, and facts as the data
values. The code then pareses through the rules, evaluting them one at a time until no data is 
added. Then the code evalutes the queries, returning those that are true, and for what values.

