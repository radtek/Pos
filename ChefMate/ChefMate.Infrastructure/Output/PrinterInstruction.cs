namespace Chefmate.Infrastructure.Output
{
    class PrinterInstruction
    {
        public string Value { get; set; }
        public bool CutAfter { get; set; }
        public bool LargeFont { get; set; }
        public bool DrawLineBefore { get; set; }
        public bool DrawLineAfter { get; set; }
    }
}
