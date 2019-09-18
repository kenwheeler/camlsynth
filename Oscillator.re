type mode =
  | Sine
  | Saw
  | Square;

type oscillator = {
  .
  setMode: mode => unit,
  setFrequency: float => unit,
  setSampleRate: float => unit,
  generate: (array(float), int) => unit,
};

let create = () => {
  let osc: oscillator = {
    val wave = ref(Sine);
    val pi = 2.0 *. acos(0.0);
    val frequency = ref(440.0);
    val phase = ref(0.0);
    val sampleRate = ref(44100.0);
    val phaseIncrement = ref(0.0);
    pub setMode = mode => {
      wave := mode;
    };
    pub setFrequency = freq => {
      frequency := freq;
      this#updateIncrement();
    };
    pub setSampleRate = rate => {
      sampleRate := rate;
      this#updateIncrement();
    };
    pub generate = (buffer, frames) => {
      let twoPI = 2. *. pi;

      switch (wave^) {
      | Sine =>
        for (i in 0 to frames - 1) {
          buffer[i] = sin(phase^);
          phase := phase^ +. phaseIncrement^;
          while (phase^ >= twoPI) {
            phase := phase^ -. twoPI;
          };
        }
      | Saw =>
        for (i in 0 to frames - 1) {
          buffer[i] = 1.0 -. 2.0 *. phase^ /. twoPI;
          phase := phase^ +. phaseIncrement^;
          while (phase^ >= twoPI) {
            phase := phase^ -. twoPI;
          };
        }
      | Square =>
        for (i in 0 to frames - 1) {
          if (phase^ <= pi) {
            buffer[i] = 1.0;
          } else {
            buffer[i] = (-1.0);
          };
          phase := phase^ +. phaseIncrement^;
          while (phase^ >= twoPI) {
            phase := phase^ -. twoPI;
          };
        }
      };
    };
    pri updateIncrement = () => {
      phaseIncrement := frequency^ *. 2.0 *. pi /. sampleRate^;
    }
  };
  osc;
};