open Settings;

type bitcrusher = {
  bitDepth: float,
  downSampling: int,
  mutable lastSample: float,
  mutable counter: int,
};

let create = (bitDepth, downSampling) => {
  bitDepth,
  downSampling,
  lastSample: 0.,
  counter: 0,
};

let bitcrush = (v, bitDepth) => {
  let bd = bitDepth -. 1.;
  let i = ref(floor(v *. (-2.) ** bd *. (-1.)));
  if (v >= 1.0) {
    i := 2. ** bd -. 1.;
  } else if (v <= (-1.0)) {
    i := (-2.) ** bd;
  };

  let out = i^ /. (-2.) ** bd *. (-1.);
  out;
};

let process = (b: bitcrusher, input: float) => {
  b.counter = b.counter + 1;
  let out = ref(0.);
  if (b.counter < b.downSampling) {
    out := b.lastSample;
  } else {
    b.counter = 0;
    let sample = bitcrush(input, b.bitDepth);
    b.lastSample = sample;
    out := sample;
  };

  out^;
};