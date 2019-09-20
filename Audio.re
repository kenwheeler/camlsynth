open Portaudio;
open Bigarray;
open Settings;

/* Time reference */
let mtime = ref(0.0);
/* Time increment by sample rate */
let mdelta = 1. /. sampleRate;

let playing = ref(false);

Random.self_init();
Portaudio.init();
let osc1 = Osc.create(Osc.Saw, 80.5, 0.5);

let deviceId = Portaudio.get_default_output_device();
let device = Portaudio.get_device_info(deviceId);

let getStream = () => {
  let outparam =
    Some({
      channels: 2,
      device: deviceId,
      sample_format: format_float32,
      latency: device.d_default_low_output_latency,
    });

  let stream = open_stream(None, outparam, sampleRate, bufferSize, []);

  stream;
};

let fill_ba = ba => {
  /* Fill the buffer */
  for (i in 0 to bufferSize - 1) {
    /* Reset mtime if it gets too big */
    if (mtime^ > Float.max_float) {
      mtime := 0.;
    };

    let data = osc1#getData(mtime^);

    /* Increment time by sample */
    mtime := mtime^ +. mdelta;

    let left = [|2 * i|];
    let right = [|2 * i + 1|];
    /* Set the data at the index */
    Genarray.set(ba, left, data);
    Genarray.set(ba, right, data);
  };
};

let stop = stream => {
  playing := false;
  stop_stream(stream);
};
let play = stream => {
  playing := true;
  /* Create array relative to buffer size */
  let dims = [|2 * bufferSize|];
  let ba = Genarray.create(float32, c_layout, dims);
  /* Fill the array on loop and write */
  while (playing^ === true) {
    fill_ba(ba);
    Portaudio.write_stream_ba(stream, ba, 0, bufferSize);
  };
  ();
};