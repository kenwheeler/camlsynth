open Settings;

type stage =
  | Off
  | On;

type pitchEnv = {
  mutable currentStage: stage,
  mutable currentLevel: float,
  mutable multiplier: float,
  mutable currentSampleIndex: int,
  mutable nextStageSampleIndex: int,
};

let create = () => {
  currentStage: Off,
  currentLevel: 1.,
  multiplier: 1.,
  currentSampleIndex: 0,
  nextStageSampleIndex: 0,
};

let calculateMultiplier = (env, startLevel, endLevel, lengthInSamples) => {
  env.multiplier =
    1.
    +. (log(endLevel) -. log(startLevel))
    /. float_of_int(lengthInSamples);
};

let enterStage = (env, nextStage, duration) => {
  env.currentStage = nextStage;
  env.currentSampleIndex = 0;
  env.nextStageSampleIndex = (
    switch (nextStage) {
    | Off => 0
    | On => int_of_float(duration *. sampleRate)
    }
  );
  switch (nextStage) {
  | Off =>
    env.currentLevel = 1.;
    env.multiplier = 1.;
  | On =>
    env.currentLevel = 4.;
    calculateMultiplier(env, 4., 1., env.nextStageSampleIndex);
  };
  ();
};

let nextSample = (env, duration) => {
  switch (env.currentStage) {
  | Off => ()
  | On =>
    if (env.currentSampleIndex == env.nextStageSampleIndex) {
      enterStage(env, Off, duration);
    };
    env.currentLevel = env.currentLevel *. env.multiplier;
    env.currentSampleIndex = env.currentSampleIndex + 1;
  };
  env.currentLevel;
};