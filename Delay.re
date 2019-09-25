open Settings;

type delay = {
  duration: float,
  gain: float,
  delayBuffer: array(float),
  mutable currentSample: int,
};

let create = (duration, gain) => {
  duration,
  gain,
  delayBuffer: Array.make(int_of_float(sampleRate *. 2.), 0.),
  currentSample: 0,
};

let process = (d: delay, input: float) => {
  let max = int_of_float(sampleRate *. 2.) - 1;
  if (d.currentSample > max) {
    d.currentSample = 0;
  };

  let delayIndex = int_of_float(sampleRate *. 2. *. d.duration);
  let dt = d.currentSample + delayIndex;
  let insertIndex = dt < max + 1 ? dt : dt mod max;

  d.delayBuffer[insertIndex] = input;

  let output = input +. d.delayBuffer[d.currentSample] *. d.gain;

  d.currentSample = d.currentSample + 1;

  output;
};