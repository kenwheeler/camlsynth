open Revery;
open Revery.UI;
open Revery.UI.Components;
open AppState;

let component = React.component("TrackFx");

let createElement =
    (
      ~appState,
      ~parentDispatch,
      ~children as _: list(React.syntheticElement),
      (),
    ) =>
  component(hooks => {
    let settingsContainer =
      Style.[
        flexGrow(1),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`Center),
        paddingHorizontal(10),
        paddingVertical(5),
        backgroundColor(Color.hex("#888888")),
        marginHorizontal(20),
        marginBottom(20),
        marginTop(-10),
        borderRadius(5.),
        height(50),
        justifyContent(`SpaceBetween),
        border(~width=1, ~color=Color.hex("#888888")),
      ];

    let sliderLabelStyle =
      Style.[
        color(Colors.black),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(12),
      ];
    (
      hooks,
      <View style=settingsContainer>
        <View>
          <Knob
            onValueChanged={v => parentDispatch(SetAttack(v))}
            value={appState.tracks[appState.activeTrack].attack}
            maximumValue=0.5
            minimumValue=0.01
          />
          <Center> <Text style=sliderLabelStyle text="ATTACK" /> </Center>
        </View>
        <View>
          <Knob
            onValueChanged={v => parentDispatch(SetDecay(v))}
            value={appState.tracks[appState.activeTrack].decay}
            maximumValue=0.99
            minimumValue=0.01
          />
          <Center> <Text style=sliderLabelStyle text="DECAY" /> </Center>
        </View>
        <View>
          <Knob
            onValueChanged={v => parentDispatch(SetSustain(v))}
            value={appState.tracks[appState.activeTrack].sustain}
            maximumValue=0.99
            minimumValue=0.01
          />
          <Center> <Text style=sliderLabelStyle text="SUSTAIN" /> </Center>
        </View>
        <View>
          <Knob
            onValueChanged={v => parentDispatch(SetRelease(v))}
            value={appState.tracks[appState.activeTrack].release}
            maximumValue=2.0
            minimumValue=0.01
          />
          <Center> <Text style=sliderLabelStyle text="RELEASE" /> </Center>
        </View>
        <View>
          <Knob
            onValueChanged={v => parentDispatch(SetFreq(v))}
            value={appState.tracks[appState.activeTrack].freq}
            maximumValue=130.81
            minimumValue=16.35
          />
          <Center> <Text style=sliderLabelStyle text="TONE" /> </Center>
        </View>
        <View>
          <Knob
            onValueChanged={v => parentDispatch(SetGain(v))}
            value={appState.tracks[appState.activeTrack].gain}
            maximumValue=1.5
            minimumValue=0.0001
          />
          <Center> <Text style=sliderLabelStyle text="GAIN" /> </Center>
        </View>
      </View>,
    );
  });