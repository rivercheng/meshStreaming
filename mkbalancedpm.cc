// -------------------- STL
#include <iostream>
#include <sstream>
#include <cmath>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModBaseT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include <OpenMesh/Tools/Decimater/ModNormalFlippingT.hh>
#include <OpenMesh/Tools/Decimater/ModProgMeshT.hh>
#include <OpenMesh/Tools/Decimater/ModIndependentSetsT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
typedef OpenMesh::Decimater::DecimaterT<Mesh> Decimater;
typedef OpenMesh::Decimater::ModNormalFlippingT<Decimater>  ModNormalFlipping;
typedef OpenMesh::Decimater::ModQuadricT<Decimater>         ModQuadric;
typedef OpenMesh::Decimater::ModProgMeshT<Decimater>        ModProgMesh;
typedef OpenMesh::Decimater::ModIndependentSetsT<Decimater> ModIndependentSets;

const unsigned int MAX_LEVEL = 31;

// ----------------------------------------------------------------------------

using namespace OpenMesh::Decimater;

template <class D>
class ModBalancerT : public OpenMesh::Decimater::ModQuadricT<D>
{
public:

    typedef OpenMesh::Decimater::ModQuadricT<D> BaseModQ;

    DECIMATING_MODULE( ModBalancerT, D, Balancer );

public:

    typedef size_t level_t;

public:

    /// Constructor
    ModBalancerT( D &_dec )
            : BaseModQ( _dec ),
            max_level_(0), n_vertices_(0)
    {
        BaseModQ::mesh().add_property( level_ );
    }


    /// Destructor
    virtual ~ModBalancerT()
    {
        BaseModQ::mesh().remove_property( level_ );
    }

public:

    static level_t calc_bits_for_roots( size_t _n_vertices )
    {
        return level_t(std::ceil(std::log((double)_n_vertices)*inv_log2_));
    }

public: // inherited

    void initialize(void)
    {
        BaseModQ::initialize();
        n_vertices_ = BaseModQ::mesh().n_vertices();
        n_roots_    = calc_bits_for_roots(n_vertices_);
    }

    virtual float collapse_priority(const CollapseInfo& _ci)
    {
        level_t newlevel = std::max( BaseModQ::mesh().property( level_, _ci.v0 ),
                                     BaseModQ::mesh().property( level_, _ci.v1 ) )+1;
        level_t newroots = calc_bits_for_roots(n_vertices_-1);

        if ( (newroots + newlevel) < MAX_LEVEL )
        {
            double err = BaseModQ::collapse_priority( _ci );

            if (err!=BaseModQ::ILLEGAL_COLLAPSE)
            {
                return float(newlevel + err/(err+1.0));
            }


        }
        return BaseModQ::ILLEGAL_COLLAPSE;
    }

    /// post-process halfedge collapse (accumulate quadrics)
    void postprocess_collapse(const CollapseInfo& _ci)
    {
        BaseModQ::postprocess_collapse( _ci );

        BaseModQ::mesh().property( level_, _ci.v1 ) =
            std::max( BaseModQ::mesh().property( level_, _ci.v0 ),
                      BaseModQ::mesh().property( level_, _ci.v1 ) ) + 1;

        max_level_ = std::max( BaseModQ::mesh().property( level_, _ci.v1 ), max_level_ );
        n_roots_   = calc_bits_for_roots(--n_vertices_);
    }

public:

    level_t max_level(void) const
    {
        return max_level_;
    }
    level_t bits_for_roots(void) const
    {
        return n_roots_;
    }


private:


    OpenMesh::VPropHandleT<level_t> level_;

    level_t max_level_; // maximum level reached
    level_t n_roots_;   // minimum bits for root nodes
    size_t  n_vertices_;// number of potential root nodes

    static const double inv_log2_;

};

template <typename D>
const double ModBalancerT<D>::inv_log2_ = 1.0/std::log(2.0);

typedef ModBalancerT<Decimater>  ModBalancer;


// ----------------------------------------------------------------------------

inline
std::string&
replace_extension( std::string& _s, const std::string& _e )
{
    std::string::size_type dot = _s.rfind(".");
    if (dot == std::string::npos)
    {
        _s += "." + _e;
    }
    else
    {
        _s = _s.substr(0,dot+1)+_e;
    }
    return _s;
}

inline
std::string
basename(const std::string& _f)
{
    std::string::size_type dot = _f.rfind("/");
    if (dot == std::string::npos)
        return _f;
    return _f.substr(dot+1, _f.length()-(dot+1));
}

// ----------------------------------------------------------------------------

void usage_and_exit(int xcode)
{
    using namespace std;

    cout << endl
    << "Usage: mkbalancedpm [-n <decimation-steps>] [-o <output>] "
    << "<input.ext>\n"
    << endl
    << "  Create a balanced progressive mesh from an input file.\n"
    << "  By default decimate as much as possible and write the result\n"
    << "  to <input>.pm\n"
    << endl
    << "Options:\n"
    << endl
    << "  -n <decimation-steps>\n"
    << "\tDetermines the maximum number of decimation steps.\n"
    << "\tDecimate as much as possible if the value is equal zero\n"
    << "\tDefault value: 0\n"
    << endl
    << "  -o <output>\n"
    << "\tWrite resulting progressive mesh to the file named <output>\n"
    << endl
    << "  -a\n"
    << "\tEnable Normal Flipping and set the maximum angle\n"
    << endl
    << "  -I\n"
    << "\tEnable Independent Sets\n"
    << endl;
    exit(xcode);
}

// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    Mesh mesh;

    int  c;
    std::string ifname, ofname;
    size_t      decstep=0;
    bool enable_modNF = false;
    bool enable_modIS = false;
    float normal_angle = -1;

    while ((c=getopt(argc, argv, "n:o:a:Ih"))!=-1)
    {
        switch (c)
        {
        case 'o':
            ofname = optarg;
            break;
        case 'n':
        {
            std::stringstream str;
            str << optarg;
            str >> decstep;
        }
        std::cerr<<decstep<<std::endl;
        break;
        case 'a':
            enable_modNF = true;
            std::cerr<<optarg<<std::endl;
            normal_angle = static_cast<float>(atof(optarg));
            std::cerr<<normal_angle<<std::endl;
            break;
        case 'I':
            enable_modIS = true;
            break;
        case 'h':
            usage_and_exit(0);
        default:
            usage_and_exit(1);
        }
    }

    if (optind >= argc)
        usage_and_exit(1);

    ifname = argv[optind];

    if (!OpenMesh::IO::read_mesh(mesh, ifname))
    {
        std::cerr << "Error loading mesh from file '" << ifname << "'!\n";
        return 1;
    }

    {
        OpenMesh::Utils::Timer t;

        Decimater decimater(mesh);

        ModProgMesh::Handle        modPM;
        ModBalancer::Handle        modB;
        ModNormalFlipping::Handle  modNF;
        ModIndependentSets::Handle modIS;


        decimater.add_binary(modPM);
        std::cout << "w/  progressive mesh module\n";
        decimater.add_priority(modB);
        std::cout << "w/  balancer module\n";

        if ( enable_modNF )
            decimater.add_binary(modNF);
        decimater.module(modNF).set_max_normal_deviation(normal_angle);
        std::cout << "w/" << (modNF.is_valid() ? ' ' : 'o')
        << " normal flipping module\n";

        if ( enable_modIS )
            decimater.add_binary(modIS);
        std::cout << "w/" << (modIS.is_valid() ? ' ' : 'o')
        << " independent sets module\n";

        std::cout << "Initialize decimater\n";
        t.start();
        if ( !decimater.initialize() )
        {
            std::cerr << "  Initialization failed!\n";
            return 1;
        }
        t.stop();
        std::cout << "  done [" << t.as_string() << "]\n";
        t.reset();

        int    rc;
        size_t nv = mesh.n_vertices();

        std::cout << "Begin decimation (#V " << nv << ")\n";
        t.start();
        if (modIS.is_valid())
        {
            Mesh::VertexIter  v_it;
            Mesh::FaceIter    f_it;

            for (f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
                if ( !mesh.status(f_it).deleted() )
                    mesh.update_normal(f_it);

            for (v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
                if ( !mesh.status(v_it).deleted() )
                {
                    mesh.status(v_it).set_locked(false);
                    mesh.update_normal(v_it);
                }

        }

        std::cerr<<"decimate to "<<decstep<<std::endl;
        rc = decimater.decimate_to(decstep);
        t.stop();

        std::cout << "\n  done [" << t.as_string() << "]\n";

        std::cout << "Bits for <tree-id, node-id>: <"
        << decimater.module(modB).bits_for_roots() << ", "
        << decimater.module(modB).max_level()  << ">"
        << std::endl;

        std::cout << "Maximum level reached: "
        << decimater.module(modB).max_level() << std::endl;

        if (ofname == "." || ofname == ".." )
            ofname += "/" + basename(ifname);
        std::string pmfname = ofname.empty() ? ifname : ofname;
        replace_extension(pmfname, "pm");

        std::cout << "Write progressive mesh data to file "
        << pmfname << std::endl;
        decimater.module(modPM).write( pmfname );
    }


    return 0;
}
