/****************************************************************************
 * Copyright (C) 2009-2015 EPAM Systems
 *
 * This file is part of Indigo toolkit.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#include "indigo_internal.h"
#include "base_cpp/cancellation_handler.h"
#include "layout/reaction_layout.h"
#include "layout/molecule_layout.h"
#include "reaction/base_reaction.h"
#include "indigo_molecule.h"
#include "indigo_reaction.h"
#include <vector>

CEXPORT int indigoLayout (int object)
{
   INDIGO_BEGIN
   {
      IndigoObject &obj = self.getObject(object);
      int i;

      if (IndigoBaseMolecule::is(obj) || obj.type == IndigoObject::SUBMOLECULE) {
         BaseMolecule &mol = obj.getBaseMolecule();
         MoleculeLayout ml(mol, self.smart_layout);
         ml.max_iterations = self.layout_max_iterations;
         ml.bond_length = 1.6f;

         TimeoutCancellationHandler cancellation(self.cancellation_timeout);
         ml.setCancellationHandler(&cancellation);

         Filter f;
         if (obj.type == IndigoObject::SUBMOLECULE)
         {
            IndigoSubmolecule &submol = (IndigoSubmolecule &)obj;
            f.initNone(mol.vertexEnd());
            for (int i = 0; i < submol.vertices.size(); i++)
               f.unhide(submol.vertices[i]);
            ml.filter = &f;
         }

         ml.make();
         std::vector<float> to_out;
         /*
         for (int i = mol.vertexBegin(); i != mol.vertexEnd(); i = mol.vertexNext(i)) {
             to_out.push_back(mol.getAtomXyz(i).x);
             to_out.push_back(mol.getAtomXyz(i).y);
         }

         for (int i = 0; i < to_out.size(); i++) {
             //printf("%d: ", i);

             float x = to_out[i];
             int sign = x < 0 ? -1 : 1;
             x = fabs(x);
             int deg = 0;
             
             if (x != 0) {
                 while (x >= 2) {
                     deg++;
                     x /= 2;
                 }
                 while (x < 1) {
                     deg--;
                     x *= 2;
                 }
             }
             printf("%d ", deg);
             if (sign > 0) printf("+"); else printf("-");
             
             printf("0");
             while (x != 0) {
                 if (x >= 1) printf("1"); else printf("0");
                 if (x >= 1) x -= 1;
                 x *= 2;
             }
             if (i & 1) printf("\n"); else printf(" ");
             
         }
      */
         if (obj.type != IndigoObject::SUBMOLECULE)
         {
            // Not for submolecule yet
            mol.clearBondDirections();
            mol.stereocenters.markBonds();
            mol.allene_stereo.markBonds();
         }

         if (obj.type != IndigoObject::SUBMOLECULE)
         {
            for (i = 1; i <= mol.rgroups.getRGroupCount(); i++)
            {
               RGroup &rgp = mol.rgroups.getRGroup(i);

               for (int j = rgp.fragments.begin(); j != rgp.fragments.end();
                        j = rgp.fragments.next(j))
               {
                  rgp.fragments[j]->clearBondDirections();
                  rgp.fragments[j]->stereocenters.markBonds();
                  rgp.fragments[j]->allene_stereo.markBonds();
               }
            }
         }
      } else if (IndigoBaseReaction::is(obj)) {
         BaseReaction &rxn = obj.getBaseReaction();
         ReactionLayout rl(rxn, self.smart_layout);
         rl.max_iterations = self.layout_max_iterations;
         rl.bond_length = 1.6f;
         rl.make();
         rxn.markStereocenterBonds();
      } else {
         throw IndigoError("The object provided is neither a molecule, nor a reaction");
      }
      return 0;
   }
   INDIGO_END(-1)
}