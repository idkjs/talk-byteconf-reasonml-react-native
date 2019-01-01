open BsReactNative;

/* create Config Module which holds the typs for route and navigation. Nav needs to be typed to resolve error when passing navigation to HomeScreen component which is throwing and error. */
module Config = {
  type route =
      |Screen(string)
      |Home;

  type navigation = StackNavigator.navigation(route);
};
let str = ReasonReact.string;
/* using 25.0 because intent to animate header fontsize */
let animatedValue = Animated.Value.create(25.0);
module Styles = {
  open Style;
  let container =
    style([flex(1.0), alignContent(Center), justifyContent(Center)]);

  /* define centerElement alias, use concat to combine with other styles. Useful when combining with styles from another module */
  let centerElement = style([textAlign(Center)]);
  /* before concat
     let header = style([fontSize(Float(25.0)), textAlign(Center), marginBottom(Pt(5.0))]); */
  let header =
    concat([
      style([fontSize(Animated(animatedValue)), marginBottom(Pt(5.0))]),
      centerElement,
    ]);
  let paragraph = concat([style([]), centerElement]);
};

/* simple animation */
/* let animation = Animated.Timing.animate(
  ~value=animatedValue,
  ~toValue=`raw(15.0),
  ~duration=500.0,
  ()
); */
/* Animation Sequences */
let animation =
Animated.sequence([|
  Animated.timing(
  ~value=animatedValue,
  ~toValue=`raw(15.0),
  ~duration=500.0,
  (),
  ),
  Animated.timing(
    ~value=animatedValue,
    ~toValue=`raw(25.0),
    ~duration=500.0,
    (),
  ),
|]);

module HomeScreen = {
  open Config;
  let component = ReasonReact.statelessComponent("HomeScreen");

let debuggerString = switch(Platform.os()){
  | Platform.IOS(_) => "Press CMD+R to reload,\n CMD+D or shake for dev menu"
  | Platform.Android => "Double tap R on your keyboard\nor shake for dev menu"
}
  let make = (~navigation:navigation , _children) => {
    ...component,
    /* execute animation function */
    didMount: _self => Animated.start(animation, ()),
    render: _self =>
      <View style=Styles.container>
        <Animated.Text style=Styles.header> {str("Welcome to React Native")} </Animated.Text>
        <Text style=Styles.paragraph>
          {str(debuggerString)}
          /* {str("Press CMD+R to reload,\n CMD+D or shake for dev menu")} */
        </Text>
        <Button title="Nav to New Route" onPress={() => navigation.push(Screen("Test String"))}/>
      </View>,
  };
};

module Screen = {
  let component = ReasonReact.statelessComponent("Screen");
  let make = (~heading, _children) =>{
    ...component,
    render: _self =>
    <View style=Styles.container>
        <Text style=Styles.paragraph>
          {str(heading)}
        </Text>
      </View>,
  }
};

module NavigationConfig = {
  open StackNavigator;
  open Config;
  type route = Config.route;
  let initialRoute = Home;

  let getScreen = (route, navigation) =>
  /* return a tuple with screenOptions as second arg in tuple
  screenOption has a bunch of properties, ie title. we pass a title value then call.
  */
  switch(route) {
    | Home => (<HomeScreen navigation/>, screenOptions(~title="Home Screen",()))
    | Screen(heading) => (<Screen heading />, screenOptions(~title=heading, ()))
  };
};

module Stack = StackNavigator.Create(NavigationConfig);
/* let app = () => <HomeScreen />; */
/* set app to Stack's navigator function which will take care of which screen to show the user first based on the initialRoute property we set in Config */
let app = Stack.navigator;