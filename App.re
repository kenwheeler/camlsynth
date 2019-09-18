open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;
open Portaudio;
open Bigarray;

let bufferSize = 512;

Console.log("Using " ++ get_version_string());
Random.self_init();
Portaudio.init();

let osc = Oscillator.create();
osc#setFrequency(220.);
let arr = Array.create_float(bufferSize);

let fill_ba = ba => {
  for (i in 0 to bufferSize - 1) {
    osc#generate(arr, bufferSize);
    let left = [|2 * i|];
    let right = [|2 * i + 1|];
    Genarray.set(ba, left, arr[i]);
    Genarray.set(ba, right, arr[i]);
  };
};

let test_bigarray = (stream, inter, batype) => {
  let dims = [|2 * bufferSize|];
  let ba = Genarray.create(batype, c_layout, dims);
  for (j in 0 to 172) {
    fill_ba(ba);
    Portaudio.write_stream_ba(stream, ba, 0, bufferSize);
  };
};

let device = Portaudio.get_device_info(4);
Console.log(
  "Device sample rate: " ++ string_of_float(device.d_default_sample_rate),
);
Console.log(
  "Device latency: " ++ string_of_float(device.d_default_low_output_latency),
);
Console.log("Device name: " ++ device.d_name);

let play = () => {
  let outparam =
    Some({
      channels: 2,
      device: 4,
      sample_format: format_float32,
      latency: device.d_default_low_output_latency,
    });

  let stream =
    open_stream(None, outparam, device.d_default_sample_rate, bufferSize, []);

  start_stream(stream);
  test_bigarray(stream, 1, float32);
  close_stream(stream);
};

let simpleButton = {
  let component = React.component("SimpleButton");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let wrapperStyle =
        Style.[
          backgroundColor(Color.rgba(1., 1., 1., 0.1)),
          border(~width=2, ~color=Colors.white),
          margin(16),
        ];

      let textHeaderStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(20),
        ];

      let textContent = "Play";
      (
        hooks,
        <Clickable onClick=play>
          <View style=wrapperStyle>
            <Padding padding=4>
              <Text style=textHeaderStyle text=textContent />
            </Padding>
          </View>
        </Clickable>,
      );
    });
};

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win = App.createWindow(app, "Welcome to Revery!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

  let innerStyle = Style.[flexDirection(`Row), alignItems(`FlexEnd)];
  let textHeaderStyle =
    Style.[
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(24),
    ];
  let element =
    <View style=containerStyle>
      <View style=innerStyle>
        <Text style=textHeaderStyle text="Welcome" />
      </View>
      <simpleButton />
    </View>;

  let _ = UI.start(win, element);
  ();
};

App.start(init);