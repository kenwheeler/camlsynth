open Settings;

type osc = {
  .
  freq: float,
  gain: float,
  getData: float => float,
};

type wave =
  | Sine
  | Saw
  | Square
  | Noise;

let create = (mode: wave, freq: float, gain: float) => {
  let ret: osc = {
    pub freq = freq;
    pub gain = gain;
    pub getData = mtime => {
      let fpt = 1. /. this#freq; /* full period time */
      let hpt = fpt /. 2.; /* half period time */
      let lt = mod_float(mtime, fpt); /* local time */
      let doublePi = Float.pi *. 2.;

      switch (mode) {
      | Sine => this#gain *. sin(2. *. doublePi *. this#freq *. mtime)
      | Square => this#gain *. (lt < hpt ? 1.0 : (-1.0))
      | Saw => this#gain *. (lt /. fpt *. 2. -. 1.0)
      | Noise => this#gain *. 1. -. Random.float(2.)
      };
    }
  };
  ret;
};