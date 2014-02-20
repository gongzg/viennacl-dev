#ifndef VIENNACL_GENERATOR_PROFILES_HPP
#define VIENNACL_GENERATOR_PROFILES_HPP

/* =========================================================================
   Copyright (c) 2010-2013, Institute for Microelectronics,
           Institute for Analysis and Scientific Computing,
           TU Wien.
   Portions of this software are copyright by UChicago Argonne, LLC.

           -----------------
 ViennaCL - The Vienna Computing Library
           -----------------

   Project Head:    Karl Rupp  rupp@iue.tuwien.ac.at

   (A list of authors and contributors can be found in the PDF manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */


/** @file viennacl/generator/profiles.hpp
 *
 * Vendor-specific parameters for the generated kernels
*/

#include <map>

#include "viennacl/ocl/device.hpp"

#include "viennacl/generator/forwards.h"

#include "viennacl/tools/shared_ptr.hpp"

#include "viennacl/generator/templates/template_base.hpp"
#include "viennacl/generator/templates/saxpy.hpp"
#include "viennacl/generator/templates/scalar_reduction.hpp"
#include "viennacl/generator/templates/vector_reduction.hpp"
#include "viennacl/generator/templates/matrix_product.hpp"

namespace viennacl{

  namespace generator{

    namespace profiles{

      using namespace viennacl::ocl;

      typedef cl_uint vendor_id_type;
      typedef cl_device_type device_type;

      typedef std::string device_name_type;
      typedef tools::shared_ptr<profile_base> profile_base_ptr;

      template<class KeyType, class ValueType>
      struct map_wrapper{
          typedef std::map<KeyType,ValueType> map_type;
          map_type map;
          ValueType & operator[](KeyType const & key){ return map[key]; }
      };

      struct expression_map : public map_wrapper<expression_key_type, profile_base_ptr>{ };
      struct device_name_map : public map_wrapper<device_name_type, expression_map>{ };
      struct device_architecture_map : public map_wrapper<device_architecture_family, device_name_map>{ };
      struct device_type_map : public map_wrapper<device_type,device_architecture_map>{ };
      struct database_type : public map_wrapper<vendor_id_type, device_type_map>{ };

      /** @brief Set a default of a generation to a particular device for a particular operation */
        inline void set_generation_default_to(database_type & map, vendor_id_type vendor_id, device_architecture_family family, expression_key_type expression, std::string const & device_name){
            map[vendor_id][CL_DEVICE_TYPE_GPU][family][""][expression] = map[vendor_id][CL_DEVICE_TYPE_GPU][family][device_name][expression];
        }

        /** @brief Set a default of a generation to a particular device for all operations */
        inline void set_all_generation_default_to(database_type & map, vendor_id_type vendor_id, device_architecture_family family, std::string const & device_name){
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_SAXPY_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_SAXPY_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(SCALAR_REDUCE_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Tx_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Nx_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TT_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NT_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TN_TYPE,4),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NN_TYPE,4),device_name);

            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_SAXPY_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_SAXPY_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(SCALAR_REDUCE_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Tx_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Nx_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TT_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NT_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TN_TYPE,8),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NN_TYPE,8),device_name);
        }

        /** @brief Initialize the database */
      static database_type init_database(){
        database_type map;

        /*---------------------------*/
        /*     GPU Defaults          */
        /*---------------------------*/
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,4)] = profile_base_ptr(new vector_saxpy(1,128,128,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,4)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,4)] = profile_base_ptr(new scalar_reduction(1, 128, 128, true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)] = profile_base_ptr(new vector_reduction(1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)] = profile_base_ptr(new vector_reduction(1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));


        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,8)] = profile_base_ptr(new vector_saxpy(1,128,128,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,8)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,8)] = profile_base_ptr(new scalar_reduction(1, 128, 128, true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)] = profile_base_ptr(new vector_reduction(1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)] = profile_base_ptr(new vector_reduction(1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] = profile_base_ptr(new matrix_product(1,8,32,8,4,4,4,1,0));

        /*---------------------------*/
        /*     CPU Defaults          */
        /*---------------------------*/
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,4)] = profile_base_ptr(new vector_saxpy(8,16,256,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,4)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,4)] = profile_base_ptr(new scalar_reduction(8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,2,1,8));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,16,8,8));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,1,4,4,128,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,1,4,4,128,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,1,4,4,128,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,1,4,4,128,0,0));


        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,8)] = profile_base_ptr(new vector_saxpy(8,16,32,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,8)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,8)] = profile_base_ptr(new scalar_reduction(8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,1,1,8));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,8,16,16));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,64,1,8,4,64,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,64,1,8,4,64,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,64,1,8,4,64,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,64,1,8,4,64,0,0));


        /*---------------------------*/
        /*     ACCELERATOR Defaults  */
        /*---------------------------*/
        //same as CPU for now
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,4)] = profile_base_ptr(new vector_saxpy(8,16,256,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,4)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,4)] = profile_base_ptr(new scalar_reduction(8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,2,1,8));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,16,8,8));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,1,4,4,128,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] = profile_base_ptr(new matrix_product(1,16,64,1,16,4,32,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] = profile_base_ptr(new matrix_product(1,16,64,1,16,4,32,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] = profile_base_ptr(new matrix_product(1,16,64,1,16,4,32,0,0));


        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,8)] = profile_base_ptr(new vector_saxpy(8,16,32,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,8)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,8)] = profile_base_ptr(new scalar_reduction(8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,1,1,8));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,8,16,16));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,64,1,8,4,64,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] = profile_base_ptr(new matrix_product(1,16,128,2,8,4,16,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] = profile_base_ptr(new matrix_product(1,16,64,1,16,4,32,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] = profile_base_ptr(new matrix_product(1,16,64,1,16,4,32,0,0));



        /*---------------------------*/
        /*     AMD                   */
        /*---------------------------*/

        //Evergreen

            //Cypress
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_SAXPY_TYPE,4)] = profile_base_ptr(new vector_saxpy(1,4,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_SAXPY_TYPE,4)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(SCALAR_REDUCE_TYPE,4)] = profile_base_ptr(new scalar_reduction(8,128,128,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,32,8,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] = profile_base_ptr(new matrix_product(4,4,64,16,4,4,8,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] = profile_base_ptr(new matrix_product(2,8,32,8,4,1,4,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] = profile_base_ptr(new matrix_product(4,8,64,16,4,4,8,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] = profile_base_ptr(new matrix_product(4,8,128,8,8,4,4,0,0));

            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_SAXPY_TYPE,8)] = profile_base_ptr(new vector_saxpy(2,1,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_SAXPY_TYPE,8)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(SCALAR_REDUCE_TYPE,8)] = profile_base_ptr(new scalar_reduction(2,256,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,64,4,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,32,16,2,2,8,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,32,8,4,1,4,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] = profile_base_ptr(new matrix_product(4,2,64,32,8,8,4,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] = profile_base_ptr(new matrix_product(2,16,64,8,2,2,4,0,0));

            //Default
            set_all_generation_default_to(map,amd_id,Evergreen,"Cypress");


         //Southern Islands
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_SAXPY_TYPE,4)] = profile_base_ptr(new vector_saxpy(1,4,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_SAXPY_TYPE,4)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(SCALAR_REDUCE_TYPE,4)] = profile_base_ptr(new scalar_reduction(8,128,128,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,32,8,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] = profile_base_ptr(new matrix_product(4,8,128,32,4,4,4,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] = profile_base_ptr(new matrix_product(2,8,32,8,4,2,4,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,16,4,4,8,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,16,4,4,8,1,0));

            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_SAXPY_TYPE,8)] = profile_base_ptr(new vector_saxpy(2,1,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_SAXPY_TYPE,8)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(SCALAR_REDUCE_TYPE,8)] = profile_base_ptr(new scalar_reduction(2,256,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,64,4,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] = profile_base_ptr(new matrix_product(2,4,128,64,4,2,2,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,32,8,4,2,4,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,128,32,2,2,2,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,128,32,2,2,2,1,0));


            //Default
            set_all_generation_default_to(map,amd_id,SouthernIslands,"Tahiti");
			
			
		map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_SAXPY_TYPE,4)] = profile_base_ptr(new vector_saxpy(1,4,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_SAXPY_TYPE,4)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(SCALAR_REDUCE_TYPE,4)] = profile_base_ptr(new scalar_reduction(8,128,128,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)] = profile_base_ptr(new vector_reduction(1,32,8,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] = profile_base_ptr(new matrix_product(4,8,128,32,4,4,4,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] = profile_base_ptr(new matrix_product(4,8,8,8,8,1,8,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,16,4,4,8,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] = profile_base_ptr(new matrix_product(4,16,64,16,4,4,8,1,0));

            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_SAXPY_TYPE,8)] = profile_base_ptr(new vector_saxpy(2,1,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_SAXPY_TYPE,8)] = profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(SCALAR_REDUCE_TYPE,8)] = profile_base_ptr(new scalar_reduction(2,256,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)] = profile_base_ptr(new vector_reduction(1,64,4,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] = profile_base_ptr(new matrix_product(2,4,128,64,4,2,2,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,16,16,8,1,4,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,128,32,2,2,2,1,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] = profile_base_ptr(new matrix_product(2,8,128,32,2,2,2,1,0));
            set_all_generation_default_to(map,amd_id,VolcanicIslands,"Hawaii");


        /*---------------------------*/
        /*     NVidia                */
        /*---------------------------*/

        //-----Fermi

          //Geforce GTX 470
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_SAXPY_TYPE,4)]      =    profile_base_ptr(new vector_saxpy(1,1,256,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_SAXPY_TYPE,4)]      =    profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(SCALAR_REDUCE_TYPE,4)]     =    profile_base_ptr(new scalar_reduction(4,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)]  =    profile_base_ptr(new vector_reduction(1,1,256,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)]  =    profile_base_ptr(new vector_reduction(1,64,4,64));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] =    profile_base_ptr(new matrix_product(1,2,64,64,8,4,2,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] =    profile_base_ptr(new matrix_product(1,16,16,16,4,1,4,1,1));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] =    profile_base_ptr(new matrix_product(1,2,32,32,8,4,2,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] =    profile_base_ptr(new matrix_product(1,32,16,8,4,8,4,1,0));

          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_SAXPY_TYPE,8)]      =    profile_base_ptr(new vector_saxpy(2,1,64,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_SAXPY_TYPE,8)]      =    profile_base_ptr(new matrix_saxpy(2,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(SCALAR_REDUCE_TYPE,8)]     =    profile_base_ptr(new scalar_reduction(2,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)]  =    profile_base_ptr(new vector_reduction(1,1,128,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)]  =    profile_base_ptr(new vector_reduction(1,16,32,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] =    profile_base_ptr(new matrix_product(1,8,64,32,2,2,8,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] =    profile_base_ptr(new matrix_product(1,16,16,16,4,1,4,1,1));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] =    profile_base_ptr(new matrix_product(1,4,128,32,4,8,4,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] =    profile_base_ptr(new matrix_product(1,4,32,16,8,4,8,0,0));

          //default
          set_all_generation_default_to(map,nvidia_id,Fermi,"GeForce GTX 470");

          //-----Kepler

          //Geforce GTX 680 (based on feedback from Paul Dufort via viennacl-support)
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_SAXPY_TYPE,4)]      =    profile_base_ptr(new vector_saxpy(1,1,256,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_SAXPY_TYPE,4)]      =    profile_base_ptr(new matrix_saxpy(1,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(SCALAR_REDUCE_TYPE,4)]     =    profile_base_ptr(new scalar_reduction(4,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,4)]  =    profile_base_ptr(new vector_reduction(1,1,256,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,4)]  =    profile_base_ptr(new vector_reduction(1,64,4,64));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,4)] =    profile_base_ptr(new matrix_product(1,2,64,64,8,4,2,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,4)] =    profile_base_ptr(new matrix_product(1,8,32,16,4,4,8,0,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,4)] =    profile_base_ptr(new matrix_product(1,4,128,32,4,8,4,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,4)] =    profile_base_ptr(new matrix_product(1,4,32,16,8,4,8,0,0));

          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_SAXPY_TYPE,8)]      =    profile_base_ptr(new vector_saxpy(2,1,64,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_SAXPY_TYPE,8)]      =    profile_base_ptr(new matrix_saxpy(2,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(SCALAR_REDUCE_TYPE,8)]     =    profile_base_ptr(new scalar_reduction(2,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,8)]  =    profile_base_ptr(new vector_reduction(1,1,128,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,8)]  =    profile_base_ptr(new vector_reduction(1,16,32,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,8)] =    profile_base_ptr(new matrix_product(1,8,64,32,2,2,8,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,8)] =    profile_base_ptr(new matrix_product(1,64,128,4,2,2,8,0,1));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,8)] =    profile_base_ptr(new matrix_product(1,4,128,32,4,8,4,1,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,8)] =    profile_base_ptr(new matrix_product(1,4,32,16,8,4,8,0,0));

          //default
          set_all_generation_default_to(map,nvidia_id,Kepler,"GeForce GTX 680");


        return map;
      }
      static database_type database = init_database();

      /** @brief If the fallback is too harsh, use a very conservative profile */
      static profile_base * handle_failure(viennacl::ocl::device const & device, expression_descriptor const & descriptor, tools::shared_ptr<profile_base> const & profile){
        //Returns default if the profile is invalid
        if(profile->is_invalid(device, descriptor.scalartype_size))
          return database.map.at(unknown_id).map.at(device.type()).map.at(UNKNOWN).map.at("").map.at(descriptor.make_key()).get();
        return profile.get();
      }

      /** @brief Get the profile for a device and a descriptor */
      static profile_base * get(viennacl::ocl::device const & device, expression_descriptor const & descriptor){
        device_type dev_type = device.type();
        vendor_id_type vendor_id = device.vendor_id();
        device_architecture_family device_architecture = device.architecture_family();
        std::string const & device_name = device.name();
        expression_key_type expression_key = descriptor.make_key();

        //std::cout << "Looking up vendor ID..." << std::endl;
        /*-Vendor ID-*/
        database_type::map_type::iterator vendor_it = database.map.find(vendor_id);
        //Vendor not recognized => global default:
        if(vendor_it==database.map.end())
          return handle_failure(device, descriptor, database.map.at(unknown_id).map.at(dev_type).map.at(UNKNOWN).map.at("").map.at(expression_key));

        /*-Device Type-*/
        //std::cout << "Looking up device type..." << std::endl;
        device_type_map::map_type::iterator device_type_it = vendor_it->second.map.find(dev_type);
        //Device type not recognized for this vendor => global default
        if(device_type_it==vendor_it->second.map.end())
          return handle_failure(device, descriptor, database.map.at(unknown_id).map.at(dev_type).map.at(UNKNOWN).map.at("").map.at(expression_key));

        /*-Device Architecture-*/
        //std::cout << "Looking up device architecture..." << std::endl;
        device_architecture_map::map_type::iterator architecture_it = device_type_it->second.map.find(device_architecture);
        if(architecture_it==device_type_it->second.map.end())
          return handle_failure(device, descriptor, database.map.at(unknown_id).map.at(dev_type).map.at(UNKNOWN).map.at("").map.at(expression_key));

        /*-Device Name-*/
        //std::cout << "Looking up device name..." << std::endl;
        device_name_map::map_type::iterator device_name_it = architecture_it->second.map.find(device_name);
        //Name not found => Vendor default
        if(device_name_it==architecture_it->second.map.end())
          return handle_failure(device, descriptor, database.map.at(vendor_id).map.at(dev_type).map.at(device_architecture).map.at("").map.at(expression_key));

        //std::cout << "Looking up expression name.." << std::endl;
        /*-Expression-*/
        expression_map::map_type::iterator expression_it = device_name_it->second.map.find(expression_key);
        //Expression not found => Vendor default
        if(expression_it==device_name_it->second.map.end())
          return handle_failure(device, descriptor, database.map.at(vendor_id).map.at(dev_type).map.at(device_architecture).map.at("").map.at(expression_key));

        //std::cout << "Device found in the database! Getting profile..." << std::endl;
        //Everything okay. Return specific profile//
        return handle_failure(device, descriptor, database.map.at(vendor_id).map.at(dev_type).map.at(device_architecture).map.at(device_name).map.at(expression_key));
      }

    }

  }

}


#endif

