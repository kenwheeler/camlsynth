open Settings;

type osc = {
  freq: float,
  gain: float,
  getData: float => float,
};

type wave =
  | Sine
  | Saw
  | Square
  | Noise;

let getData =
    (mode: wave, freq: float, gain: float, mtime: float, phase: float) => {
  let fpt = 1. /. freq; /* full period time */
  let hpt = fpt /. 2.; /* half period time */
  let lt = mod_float(mtime, fpt); /* local time */
  let doublePi = Float.pi *. 2.;

  switch (mode) {
  | Sine => sin(2. *. doublePi *. freq *. mtime +. phase) *. gain
  | Square => (lt < hpt ? 1.0 : (-1.0)) *. gain
  | Saw => (lt /. fpt *. 2. -. 1.0) *. gain
  | Noise => 1. -. Random.float(2.) *. gain
  };
};

/* let updateAngleDelta = osc => {
     let cycles = osc.freq /. sampleRate;
     osc.angleDelta = cycles *. 2.0 *. (Float.pi *. 2.);
   }; */