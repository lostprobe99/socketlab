class Program
{
    // crc16-ccitt
    static int crc16(byte[] data, int len)
    {
        ushort crc_reg = 0x0000, tmp = 0;
        byte j, b = 0;
        int left = 0, right = len;

        while(len-- > 0) {
            b = data[left++];
            crc_reg ^= (ushort)(b << 8);
            for(j = 0; j < 8; j++) {
                tmp = (ushort)(crc_reg & 0x8000);
                crc_reg <<= 1;
                if(tmp != 0)
                    crc_reg ^= 0x1021;
            }
        }
        return (int)crc_reg;
    }
    static void Main(string[] args) {
        byte[] frame = {0x41, 0x00, 0x42, 0x00, 0x43, 0x00, 0x0A, 0x00};
        var len = frame.Length;
        System.Console.WriteLine($"{crc16(frame, len):X4}");    // 0x4ad8
    }
}