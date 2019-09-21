let bufferSize = 1024;
let sampleRate = 44100.;
let tempo = 60.;

let oneBeatInMilliseconds = 60000. /. tempo;
let oneBeatInSeconds = oneBeatInMilliseconds /. 1000.;

let oneStepInSeconds = oneBeatInSeconds /. 4.;

let sample = 1. /. sampleRate;
let step = oneStepInSeconds *. sampleRate;