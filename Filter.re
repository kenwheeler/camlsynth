type mode =
  | LowPass
  | HighPass;

type filter = {
  a0: float,
  a1: float,
  a2: float,
  b1: float,
  b2: float,
  fc: float,
  q: float,
  peakGain: float,
  mutable z1: float,
  mutable z2: float,
};

let process = (f: filter, input: float) => {
  let output = input *. f.a0 +. f.z1;
  f.z1 = input *. f.a1 +. f.z2 -. f.b1 *. output;
  f.z2 = input *. f.a2 -. f.b2 *. output;
  output;
};

let create = (mode, cutoff, q, peakGain) => {
  let k = tan(Float.pi *. cutoff);
  switch (mode) {
  | LowPass =>
    let norm = 1. /. (1. +. k /. q +. k *. k);
    let a0 = k *. k *. norm;
    let a1 = 2. *. a0;
    let a2 = a0;
    let b1 = 2. *. (k *. k -. 1.) *. norm;
    let b2 = (1. -. k /. q +. k *. k) *. norm;
    {a0, a1, a2, b1, b2, fc: cutoff, peakGain, q, z1: 0., z2: 0.};
  | HighPass =>
    let norm = 1. /. (1. +. k /. q +. k *. k);
    let a0 = 1. *. norm;
    let a1 = (-2.) *. a0;
    let a2 = a0;
    let b1 = 2. *. (k *. k -. 1.) *. norm;
    let b2 = (1. -. k /. q +. k *. k) *. norm;
    {a0, a1, a2, b1, b2, fc: cutoff, peakGain, q, z1: 0., z2: 0.};
  };
};