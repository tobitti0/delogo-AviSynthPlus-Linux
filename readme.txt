-----------------------------------------------------------------------
      ���ߐ����S �t�B���^ for AviSynth 2.5  ver 0.05a  by MakKi
-----------------------------------------------------------------------

�y�@�\�z

  BS�CS�ł悭��������悤�Ȕ��������S��t���܂��͏������܂��B

�y�����z

  �E���S�t��
        AddLOGO(logofile="", logoname="",
                  pos_x=0, pos_y=0, depth=128,
                  yc_y=0,  yc_u=0,  yc_v=0,
                  start=0, fadein=0, fadeout=0, end=-1,
                  interlaced=false)
  �E���S����
        EraseLOGO(logofile="", logoname="",
                  pos_x=0, pos_y=0, depth=128,
                  yc_y=0,  yc_u=0,  yc_v=0,
                  start=0, fadein=0, fadeout=0, end=-1,
                  interlaced=false)

     logofile         : ���S�t�@�C�����i�ȗ��s�j  *.ldp�A*.lgd�̂ǂ�����g�p�ł��܂��B
     logoname         : ���S��        �i�ȗ��j    �ȗ������ꍇ�̓��S�f�[�^�t�@�C���̐擪�ɕۑ�����Ă��郍�S���g�p���܂��B
     pos_x, pox_y     : �ʒu����                    1/4�s�N�Z���P�ʂɒ������܂��B
     yc_y, yc_u, yc_v : �F����                      ���S�̐F�𒲐����܂��B
     depth            : �s�����x�i�[�x�j����        128��100%�Ƃ��Ē������܂��B
     start, end       : �J�n��I���t���[��           �t���[���ԍ��Ŏw�肵�Ă��������Bend<start�̎���start�ȍ~�̑S�Ẵt���[���Ŏ��s����܂��B
     fadein, fadeout  : �t�F�[�h�t���[����          ���S���t�F�[�h�����܂��B
     interlaced       : �C���^�[���[�X�t���O        YV12���C���^�[���[�X�Ƃ��Ĉ����܂��BYUY2�ł͖�������܂��B

  start, end, interlaced �ȊO�̃p�����[�^��AviUtl�p���ߐ����S�t�B���^�̃p�����[�^�Ƌ��ʂł��B
  ���S�f�[�^�̍쐬�ɂ�AviUtl�p���S��̓v���O�C�����g�p���Ă��������B

  ��YUY2,YV12��p�ł��B�K�v�ɉ����ČĂяo���O�ɐF��ԕϊ����Ă��������B
  ��AviSynth 2.0x�n�Ŏg�p����ɂ́Awarpsharp�v���O�C���t����LoadPlginEx.dll���g�p���Ă��������B
        warpsharp�z�z�ꏊ�Fhttp://www.geocities.co.jp/SiliconValley-PaloAlto/2382/


�y���Ӂz

  ���̃v���O�����̓t���[�\�t�g�E�F�A�ł���AGPL���C�Z���X�ɂĒ񋟂���܂��B
  ���̃v���O�����ɂ���đ��Q�𕉂����ꍇ�ł��A��҂͐ӔC�𕉂��܂���B
�@�v���O�����̃\�[�X�R�[�h�͔z�z���ɂČ��J���Ă��܂��B

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA,
    or visit http://www.gnu.org/copyleft/gpl.html .


�y�z�z���z

  MakKi's SoftWare
  http://mksoft.hp.infoseek.co.jp/

�y�X�V�����z

  2008/01/07   ver 0.05a �EYUY2�̎��A���[�������ł��Ă��Ȃ������o�O���C��
  2008/01/07   ver 0.05  �E���S�t�@�C���̍ő�f�[�^���g���ɑΉ�
  2007/06/06   ver 0.04  �Edepth�������Ȃ��ꍇ������o�O���C��
                         �Eyc_y,yc_u,yc_v�̌������ア�o�O���C��
  2007/03/21   ver 0.03  �E�S����������
                         �E�F���̌v�Z�ł̕s�v�ȏ������폜
                         �E����������AviUtl�łƂقړ����ɂ���
                         �EYV12�ɑΉ�
  2004/02/11   ver 0.02  �E���S�̐F�̒����@�\��ǉ�
  2003/11/05   ver 0.01d �E�ׂ��ȃo�O�C��
  2003/11/02   ver 0.01c �Epos_x,pos_y�̋������������������̂��C��
                         �ECrop���Ă��鎞���S�������o�O�C��
  2003/11/01   ver 0.01b �Epos_x,pos_y�ɕ��̒l�����Ă��������ɂȂ��Ă����o�O�C��
  2003/10/26   ver 0.01a �Epos_x,pos_y��-200�����̎��G���[�ɂȂ邱�Ƃ��������̂��C��
  2003/10/01   ver 0.01  �E���J


mailto:makki_d210@yahoo.co.jp
