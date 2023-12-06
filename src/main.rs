mod console;

fn main() {
    let mut console = console::Console::new();
    console.load_cartridge("roms/nestest.nes");
    console.run();
}
