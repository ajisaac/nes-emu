use crate::apu::Apu;
use crate::cartridge::Cartridge;
use crate::cpu::Cpu;
use crate::ppu::Ppu;

/// console owns all of the smaller components.
/// you load a cartridge into a console.
/// you can reset a cpu

pub struct Console {
    /// cpu is the 6502 processor
    cpu: Cpu,
    /// ppu is the picture processing unit
    ppu: Ppu,
    /// apu is the audio processing unit
    apu: Apu,
    /// cartridge is the game cartridge
    cartridge: Cartridge,
    /// ram is the 2kb of ram
    /// ram: [u8; 2048],
    /// controller1 is the first controller
    /// controller1: Controller,
    /// controller2 is the second controller
    /// controller2: Controller,
    /// mapper is the cartridge mapper
    /// mapper: Mapper,
}

impl Console {
    pub(crate) fn new() -> Self {
        /// create a new console
        /// initialize the cpu
        /// initialize the ppu
        /// initialize the apu
        /// set up controllers
        /// load the cartridge
        todo!()
    }

    pub(crate) fn run(&self) {
        todo!()
    }

    pub(crate) fn load_cartridge(&self, p0: &str) {
        todo!()
    }

    pub(crate) fn reset(&self) {
        /// reset the cpu
        todo!()
    }

    pub(crate) fn step(&self) {
        /// step the cpu
        /// step the ppu
        /// step the apu
        /// step the mapper
        todo!()
    }

    /// what is a step frame?
    pub(crate) fn step_frame(&self) {
        /// step the cpu
        /// step the ppu
        /// step the apu
        /// step the mapper
        todo!()
    }

    pub(crate) fn step_seconds(&self) {
        ///?
        todo!()
    }

    pub(crate) fn buffer(&self) {
        ///?
        todo!()
    }

    pub(crate) fn save(&self) {
        ///?
        todo!()
    }

}
