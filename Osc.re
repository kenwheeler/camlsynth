open Settings;

type osc = {table: array(float)};

type wave =
  | Sine
  | Saw;

let generateTable = wave => {
  let table = Array.create_float(tableSize);

  for (i in 0 to tableSize - 1) {
    switch (wave) {
    | Sine =>
      table[i] =
        sin(float_of_int(i) /. float_of_int(tableSize) *. mPI *. 2.)
    | Saw =>
      table[i] = 1.0 -. 2.0 *. float_of_int(i) /. float_of_int(tableSize)
    };
  };

  table;
};

let create = () => {
  let ret = {table: generateTable(Sine)};
  ret;
};