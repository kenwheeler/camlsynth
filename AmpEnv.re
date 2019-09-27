open Settings;

type stage =
  | Off
  | On;

type ampEnv = {
  mutable currentStage: stage,
  mutable currentLevel: float,
  mutable multiplier: float,
  mutable currentSampleIndex: int,
  mutable nextStageSampleIndex: int,
};

let create = () => {
  currentStage: Off,
  currentLevel: 4.,
  multiplier: 1.0,
  currentSampleIndex: 0,
  nextStageSampleIndex: 0,
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