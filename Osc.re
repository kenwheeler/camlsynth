open Settings;

type wave =
  | Sine
  | Saw
  | Square
  | Noise;
type osc = {
  mode: wave,
  freq: float,
  gain: float,
};

let getData = (mtime, osc) => {
  let fpt = 1. /. osc.freq; /* full period time */
  let hpt = fpt /. 2.; /* half period time */
  let lt = mod_float(mtime, fpt); /* local time */
  let doublePi = Float.pi *. 2.;

  switch (osc.mode) {
  | Sine => osc.gain *. sin(2. *. doublePi *. osc.freq *. mtime)
  | Square => osc.gain *. (lt < hpt ? 1.0 : (-1.0))
  | Saw => osc.gain *. (lt /. fpt *. 2. -. 1.0)
  | Noise => osc.gain *. 1. -. Random.float(2.)
  };
};
